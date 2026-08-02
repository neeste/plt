/* wplt.c - WinMain for PLT with win32 */

#include <stdio.h>
#include <ctype.h>
#include <io.h>
#include <windows.h>
#include <windowsx.h>
#include <winspool.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "wplt.h"
#include "../out/output.h"


#ifndef mgw
#include <htmlhelp.h>
#endif
#ifndef WIN32
#define _access access
#define _hypot  hypot
#define _strdup strdup
#endif

#define HELPFILE	"wplt.chm"
#define NAMESIZE	256	/* maximum filename length */
#define MAX_MULTI	2048
#define ESC		27
#define DISTANCE(p,q)	_hypot(p.x-q.x,p.y-q.y)

char	*inq_logfn();
int     errcnt(void);
int     noext(char *);
int     outfile(char *);
int     win_done();
int	inq_page();
void    fatal();
void    disp_ofix();
void    disp_zoom(POINTS, int);
void    info_line(HDC, POINTS, int);
void    newext(char *, char *);
void    set_page(int);
void    tplt(int, char **);
void    version(void);
void    win_errs(void (*)(int, char *));
void    win_page(void (*bp)(), void (*ep)());
void    win_quit();
void    win_title(char *);
POINTS  disp_move(POINTS, POINTS);

static char     file_name[MAX_PATH] = "";
static char     edit_cmnd[20 + MAX_PATH];
static int	num_errs = 0;
static int	end_page = 0;
static int	file_time = 0;
static int	print_mode = 0;
static int	zoom_level = 0;

#ifdef NEVER
int             
isatty(int filno)
{
    return (0);
}

long            
tell(int fd)
{
    return (0);
}
#endif

/****************************************************************************
 *  WIN32 FUNCTIONS:
 ****************************************************************************/
HDC     GetPrinterDC(void);

static void msg_loop();

static int  loop_flag = 0;
static int  exit_flag = 0;
static int  file_flag = 0;
static int  resize_flag = 1;

static char hfn[MAX_PATH];
static int xwin, ywin;
static FARPROC lpAbout;
static HANDLE  hInst = 0;
static HANDLE  curCross;
static HANDLE  hAccel;
static HBITMAP hNBM;
static HDC     hDCDrw = 0;
static HDC     hdcPrint = 0;
static HWND    hWnd = 0;
static RECT    lpSize = {0, 0, 0, 0};
static POINT   MousePos;

/****************************************************************************/

static void
beg_pg()
{
    RECT r;

    GetClientRect(hWnd, &r);
    InvalidateRect(hWnd, &r, FALSE);
}

static void
end_pg()
{
    if (print_mode) {
	file_flag = REPLOT;	// fixes failure to update after printing
	print_mode = 0;
	return;
    }
    if (end_page) {
	return;
    }
    if (!exit_flag && !file_flag) {
        UpdateWindow(hWnd);
        if (errcnt() > num_errs) {
	    char str[256], *msg, *question, *caption;
	    int mb_type;
	    caption = "PLT log";
	    if (_access(inq_logfn(), 0) == 0) {
		question = " View log file?";
		mb_type = MB_YESNO;
	    } else {
		question = "";
		mb_type = MB_OK;
	    }
	    num_errs = errcnt();
	    msg = (num_errs == 1) ? "message was" : "messages were";
	    sprintf(str, "%d %s written.%s", num_errs, msg, question);
	    if (MessageBox(hWnd, str, caption, mb_type) == IDYES) {
		file_flag = VIEW_LOG;
		return;
	    }
	}
        loop_flag = 1;
        msg_loop();
    }
}

/****************************************************************************/

static void
msg_loop()
{
    MSG   msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(hWnd, hAccel, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (!loop_flag)
            break;
    }
}

void
win_reset(int file_cmd)
{
    int cx, cy, wx, wy;
    RECT r;

    file_flag = file_cmd;
    loop_flag = 0;
    if ((file_cmd == UPDATE) || (zoom_level > 0))
	return;
    GetClientRect(hWnd, &r);
    cx = r.right - r.left;
    cy = r.bottom - r.top;
    GetWindowRect(hWnd, &r);
    wx = r.right - r.left;
    wy = r.bottom - r.top;
    if (cx > cy) {
        wy += (int) (cx * 8.0 / 10.5 - cy + 0.5);
    } else {
        wx += (int) (cy * 8.0 / 10.5 - cx + 0.5);
    }
    SetWindowPos(hWnd, HWND_TOP, 0, 0, wx, wy, SWP_NOMOVE);
}

static int
win_file_time(char *fn)
{
    FILE *fp;
    struct _stat statb;

    fp = fopen(fn, "r");
    if (fp == NULL || _fstat(_fileno(fp), &statb))
	statb.st_mtime = 0;
    if (fp != NULL)
        fclose(fp);
    return ((int) statb.st_mtime);	/* file modification time */
}

static void
remove_path_element(char *pn)
{
    char    *e;

    e = &pn[strlen(pn) - 1];
    /* ignore trailing slash */
    if (e > pn) {
        if (*e == '/' || *e == '\\') {
            e--;
        }
    }
    /* back up to previous slash */
    while (e > pn) {
	if (*e == '/' || *e == '\\') {
	    e++;
	    break;
	}
        e--;
    }
    *e = '\0';
}

static void
make_help_path(char *efn)
{
    /* look in current directory */
    strcpy(hfn, HELPFILE);
    if (_access(hfn, 0) == 0) {
        return;
    }
    /* extract EXE directory */
    strcpy(hfn, efn);
    remove_path_element(hfn);
    /* append help file name */
    strcat(hfn, HELPFILE);
    /* look in EXE directory */
    if (_access(hfn, 0) == 0) {
        return;
    }
    remove_path_element(hfn);
    remove_path_element(hfn);
    strcat(hfn, HELPFILE);
    /* look in EXE parent directory */
    if (_access(hfn, 0) == 0) {
        return;
    }
    /* help file not found */
    strcpy(hfn, HELPFILE);
}

/*
 *  OutMenu() - called on WM_COMMAND messages
 */
void
OutMenu(HWND hWindow, INT idval)
{
    switch (idval) {
    case ENTER:
        loop_flag--;
        break;

    case EXIT:
        PostMessage(hWindow, WM_CLOSE, 0, 0L);
        break;

    case SAVE_FILE:
    case EDIT_FILE:
    case PRINT_FILE:
    case BATCH_PRINT:
    case VIEW_LOG:
        file_flag = idval;
        loop_flag = 0;
        break;

    case SET_SIZE:
        SetWindowPos(hWindow, HWND_TOP, 0, 0, xwin, ywin, SWP_NOMOVE);
        break;

    case OPEN_FILE:
    case REPLOT:
    case COPY_CLIP:
    case UPDATE:
        zoom_level = 0;
        disp_zoom(MAKEPOINTS(zoom_level), zoom_level);
	win_reset(idval);
        break;

    case MN_HELP_ABOUT:
        DialogBox(hInst, MAKEINTRESOURCE(ABOUTBOX), hWindow,
            (DLGPROC)lpAbout );
        break;

#ifndef mgw
    case MN_HELP_MANUAL:
        HtmlHelp( hWindow, hfn, 0, 0 );
        break;
#endif
    }
}

static void
out_paint(HWND hw, PAINTSTRUCT ps)
{
    INT  x, y, w, h;
    HDC hDCDsp;
    RECT r;

    hDCDsp = GetDC(hw);

    x = ps.rcPaint.left;
    y = ps.rcPaint.top;
    w = ps.rcPaint.right - ps.rcPaint.left;
    h = ps.rcPaint.bottom - ps.rcPaint.top;
    if (h == 0 || w == 0)
	return;
    BitBlt(hDCDsp, x, y, w, h, hDCDrw, x, y, SRCCOPY);

    GetClientRect(hw, &r);
    if (r.right > lpSize.right) {
    	x = r.left;
        r.left = lpSize.right;
        FillRect(hDCDsp, &r, (HBRUSH) GetStockObject(BLACK_BRUSH));
        r.right = r.left;
        r.left = x;
    }
    if (r.bottom > lpSize.bottom) {
        r.top = lpSize.bottom;
        FillRect(hDCDsp, &r, (HBRUSH) GetStockObject(BLACK_BRUSH));
    }

    ReleaseDC(hw, hDCDsp);
}

/*
 * liboutWndProc() - Processes messages for "libout" window
 */
LRESULT CALLBACK
liboutWndProc(HWND hWindow, UINT message, WPARAM wParam, LONG lParam)
{
    double	    dp;
    int		    ft, info_mode;
    PAINTSTRUCT	    ps;
    POINT	    curpoint;
    POINTS	    p1;
    static int	    drag = 0;
    static POINTS    p0 = {0,0};

    switch (message) {

    case WM_COMMAND:
        OutMenu(hWindow, GET_WM_COMMAND_ID(wParam, lParam));
        break;

    case WM_SIZE:
        if (zoom_level == 0)
            win_reset(REPLOT);
        break;

    case WM_ACTIVATE:
        if (!GetSystemMetrics(SM_MOUSEPRESENT)) {
            if (GET_WM_ACTIVATE_STATE(wParam, lParam)) {
                if (GET_WM_ACTIVATE_HWND(wParam, lParam) != hWindow) {
                    curpoint.x = MousePos.x;
                    curpoint.y = MousePos.y;
                    ClientToScreen(hWindow, (LPPOINT)&curpoint);
                    SetCursorPos(curpoint.x, curpoint.y);
                    SetCursor(curCross);
                    ShowCursor(GET_WM_ACTIVATE_STATE(wParam, lParam));
                }
            } else {
                ShowCursor(wParam);
            }
        }
        if (wParam && (!HIWORD(lParam))) {
            SetFocus(hWindow);
	}
	if (drag) {
	    disp_ofix();
            win_reset(UPDATE);
	    drag = 0;
	} else if (file_time && *file_name) {
	    ft = win_file_time(file_name);
   	    if (!ft || !file_time || ft != file_time) {
	        win_reset(UPDATE);
	    }
	}
        break;

    case WM_PAINT:
        BeginPaint(hWindow, (LPPAINTSTRUCT)&ps);
        out_paint(hWindow, ps);
        EndPaint(hWindow, (LPPAINTSTRUCT)&ps);
        break;

    case WM_LBUTTONDOWN:
        p0 = MAKEPOINTS(lParam);
	info_mode = (wParam & 8) ? 2 : (wParam & 4) ? 1 : 0;
        info_line(GetDC(hWindow), p0, info_mode);
	drag = 0;
	break;

    case WM_LBUTTONDBLCLK:
	p0 = MAKEPOINTS(lParam);
	zoom_level = zoom_level ? 0 : 2;
	disp_zoom(p0, zoom_level);
	win_reset(UPDATE);
	break;

    case WM_LBUTTONUP:
	if (drag) {
	    disp_ofix();
            win_reset(UPDATE);
	    drag = 0;
	}
	break;

    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
	if (wParam & 4) {
	    if (zoom_level > 0)
		zoom_level--;
	} else {
	    zoom_level++;
	}
        disp_zoom(MAKEPOINTS(lParam), zoom_level);
        win_reset(UPDATE);
	break;

    case WM_NCLBUTTONDOWN:
    case WM_NCRBUTTONDOWN:
        win_reset(UPDATE);
        return(DefWindowProc(hWindow, message, wParam, lParam));

    case WM_MOUSEMOVE:
	if (wParam & 1) {
            p1 = MAKEPOINTS(lParam);
	    dp = DISTANCE(p1,p0);
	    if (drag || (dp > 8 && dp < 80)) {
		p0 = disp_move(p1, p0);
		win_reset(UPDATE);
		drag = 1;
	    }
	} else if (drag) {
	    disp_ofix();
            win_reset(UPDATE);
	    drag = 0;
	}
	break;

    case WM_KEYDOWN:
	if (wParam == VK_HOME) {
	    set_page(1);
	    win_reset(UPDATE);
	} else if (wParam == VK_END) {
	    end_page = 1;
            loop_flag--;
	} else if (wParam == VK_PRIOR) {
	    set_page(inq_page() - 1);
	    win_reset(UPDATE);
    	} else if (wParam == VK_NEXT) {
            loop_flag--;
    	}
	sound_stop();
	break;

    case WM_DESTROY:
        hWnd = 0;
        exit_flag = 1;
        PostQuitMessage(0);
	break;

    default:
        return(DefWindowProc(hWindow, message, wParam, lParam));
    }
    return(0);
}

/*
 *  FUNCTION: AboutDlgProc(HWND, WORD, WORD, LONG)
 *
 *  PURPOSE:  Processes messages for "About" dialog box
 *
 *  MESSAGES:
 *
 *      WM_INITDIALOG - initialize dialog box
 *      WM_COMMAND    - Input received
 */

LRESULT CALLBACK
AboutDlgProc(HWND hDlg, WORD message, WORD wParam, LONG lParam)
{
    if(message == WM_COMMAND) {
        EndDialog(hDlg, TRUE);
        return(TRUE);
    }
    if (message == WM_INITDIALOG)
        return(TRUE);
    else
        return(FALSE);
}

static void
create_DCDrw()
{
    HDC hDCDsp;
    RECT r;
    int xpix, ypix;

    if (hDCDrw) {
        DeleteDC(hDCDrw);
        DeleteObject(hNBM);
    }
    GetWindowRect(hWnd, (LPRECT)&r);
    xwin = r.right - r.left;
    ywin = r.bottom - r.top;
    GetClientRect(hWnd, (LPRECT)&lpSize);
    xpix = lpSize.right - lpSize.left;
    ypix = lpSize.bottom - lpSize.top;
    hDCDsp = GetDC(hWnd);
    hDCDrw = CreateCompatibleDC(hDCDsp);
    hNBM = CreateCompatibleBitmap(hDCDsp,xpix,ypix);
    (void) SelectObject(hDCDrw, hNBM);
    ReleaseDC(hWnd, hDCDsp);
    FillRect(hDCDrw, &lpSize, (HBRUSH) GetStockObject(BLACK_BRUSH));

    win_DC(hDCDrw, lpSize.left, lpSize.bottom, lpSize.right, lpSize.top);
}

int
win_init(HINSTANCE hInstance, char *title)
{
    int         ws;
    LPSTR       cn = "libout";
    WNDCLASS    OutClass;

    if (hInst == 0) {
        hInst = hInstance;
        hAccel = LoadAccelerators(hInst, (LPSTR)"MAINACC");
        lpAbout = MakeProcInstance((FARPROC)AboutDlgProc, hInst);
        curCross = LoadCursor(NULL, IDC_CROSS);
        OutClass.lpszClassName = cn;
        OutClass.lpfnWndProc   = liboutWndProc;
        OutClass.style         = CS_DBLCLKS;
        OutClass.hInstance     = hInstance;
        OutClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(WPLT_ICON));
        OutClass.hCursor = curCross;
        OutClass.hbrBackground = NULL;
        OutClass.lpszMenuName  = MAKEINTRESOURCE(WPLT);
        OutClass.cbClsExtra    = 0;
        OutClass.cbWndExtra    = 0;
        if(!RegisterClass(&OutClass))
            return(FALSE);
    }
    if (hWnd == 0) {
        if (resize_flag)
            ws = WS_OVERLAPPEDWINDOW;
        else
            ws = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
        ywin = GetSystemMetrics(SM_CYSCREEN) * 7 / 8,
        xwin = (int) ((ywin - 40) * 10.5 / 8.0 + 0.5);
        hWnd = CreateWindow(cn, (LPSTR) title, ws, CW_USEDEFAULT, 0,
            xwin, ywin, (HANDLE) NULL, (HANDLE) NULL, (HANDLE) hInst, NULL);
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    create_DCDrw();

    win_page(beg_pg, end_pg);
    exit_flag = file_flag = 0;
    loop_flag = 0;
    return(TRUE);
}

void
win_title(char *s)
{
    char title [MAX_PATH], *p;

    // strip path
    p = s + strlen(s) - 1;
    while (p >= s && *p != '/' && *p != '\\' && *p != ':')
        p--;
    s = p + 1;

    sprintf(title, "PLT - %s", s);
    SetWindowText(hWnd, title);
    file_flag = 0;
}

int
win_done()
{
    if (file_flag) {
        if (file_flag == REPLOT)
	    create_DCDrw();
        file_flag = 0;
    }
    return (exit_flag);
}

int
win_wait()
{
    if (!exit_flag && !file_flag) {
        loop_flag = -1;
        msg_loop();
    }
    end_page = 0;
    return (file_flag);
}

void
win_mesg(char *caption, char *msg)
{
    MessageBox(hWnd, msg, caption, MB_OK);
}

int
inq_break(int page)
{
    if (exit_flag || file_flag) {
	return (1);
    }
    return (0);
}

#ifdef NEVER
void
set_page(int page)
{
    break_page = page;
}

int
inq_page()
{
    return (break_page);
}
#endif

void
win_quit()
{
    if (hWnd && hDCDrw) {
        DeleteDC(hDCDrw);
        DeleteObject(hNBM);
        hDCDrw = 0;
    }
}

int
win_open(char *title, char *filter, char *fn)
{
    OPENFILENAME    ofn;

    // Fill out the OPENFILENAME struct
    ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
    ofn.lStructSize = sizeof( OPENFILENAME );
    ofn.hwndOwner = hWnd;
    ofn.hInstance = hInst;
    strcpy(fn, "");
    ofn.lpstrFile = fn;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_HIDEREADONLY;
    ofn.lpstrFilter = filter;
    ofn.lpstrTitle = title;

    // Use the common dialog
    return GetOpenFileName( &ofn );
}

int
win_multi_open(char *title, char *filter, char *fn)
{
    OPENFILENAME    ofn;

    // Fill out the OPENFILENAME struct
    ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
    ofn.lStructSize = sizeof( OPENFILENAME );
    ofn.hwndOwner = hWnd;
    ofn.hInstance = hInst;
    strcpy(fn, "");
    ofn.lpstrFile = fn;
    ofn.nMaxFile = MAX_MULTI;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_HIDEREADONLY
	| OFN_ALLOWMULTISELECT | OFN_EXPLORER;
    ofn.lpstrFilter = filter;
    ofn.lpstrTitle = title;

    // Use the common dialog
    return GetOpenFileName( &ofn );
}

/****************************************************************************/

static void
pr_beg_pg()
{
    (void) StartPage(hdcPrint);
}

static void
pr_end_pg()
{
    (void) EndPage(hdcPrint);
}

void
print_plot(char *pfn, int multi)
{
    char           *av[4];
    int             ac = 0;
    int x, y;
    static DOCINFO  di = {sizeof(DOCINFO), "WPLT", NULL};

    print_mode = 1;
    if (multi == 0 || multi == 1) {
	hdcPrint = GetPrinterDC();
    }
    if (hdcPrint) {
	if (multi == 0 || multi == 1) {
	    x = (int) (GetDeviceCaps(hdcPrint, LOGPIXELSX) * 8.0 + 0.5);
	    y = (int) (GetDeviceCaps(hdcPrint, LOGPIXELSY) * 10.5 + 0.5);
	    win_DC(hdcPrint, 0, y, x, 0);
	    win_page(pr_beg_pg, pr_end_pg);
	}
	if (multi == 0 || multi == 2) {
            (void) StartDoc(hdcPrint, &di);
	    av[ac++] = "wplt";
	    av[ac++] = pfn;
	    tplt(ac, av);
	    EndDoc(hdcPrint);
	}
	if (multi == 0 || multi == 3) {
	    DeleteDC(hdcPrint);
	    hdcPrint = 0;
	    file_flag = REPLOT;
	    win_page(beg_pg, end_pg);
	}
    }
}

HDC
GetPrinterDC(void)
{

    PRINTDLG pd;  // initialization data

    pd.lStructSize = sizeof(pd);
    pd.hwndOwner = hWnd;
    pd.hDevMode = NULL;
    pd.hDevNames = NULL;
    pd.hDC = 0;
    pd.Flags = PD_RETURNDC;
    pd.nFromPage = 1;
    pd.nToPage = 1;
    pd.nMinPage = 1;
    pd.nMaxPage = 1;
    pd.nCopies = 1;
    if (PrintDlg(&pd)) {
	GlobalFree(pd.hDevMode);
	GlobalFree(pd.hDevNames);
	return (pd.hDC);
    }

    return(0);
}

/****************************************************************************/

static OPENFILENAME ofn;
static char     sfn[MAX_PATH] = "plt.eps";
static char    *filter = 
    "Encapsulated PostScript (eps)\0" "*.eps\0"
    "PostScript (ps)\0" "*.ps\0"
    "Computer Graphics Metafile (cgm)\0" "*.cgm\0"
    "Enhanced Metafile (emf)\0" "*.emf\0"
    "Scalable Vector Graphics (svg)\0" "*.svg\0"
    "Merged Plt Include Files (mpl)\0" "*.mpl\0"
    "All Files\0" "*.*\0";
static char *dext[] = {"", "eps", "ps", "cgm", "emf", "emf", "svg", "mpl"};
static char *mode[] = {"", "-m5", "-m5", "-m6", "-m7", "-m8", "-m7", "-i"};
static UINT mergfile = 6;
static UINT prevfilt = 0;

static int
get_nums(char *s, int *n, int m)
{
    int i = 0;

    while (*s && i < m) {
	while (*s == ' ')
	    s++;
        n[i++] = atoi(s);
	while (*s && *s != ' ')
	    s++;
    }
    
    return (i);
}

static void
eps_fix(char *sfn)
{
    FILE *fp1, *fp2;
    char  tfn[MAX_PATH];
    char  line[256];
    int    n1 = 14, n2 = 8, nb = 0, np = 0, b[4], p[1];
    static char *s1 = "%%BoundingBox:";
    static char *s2 = "%%Pages:";
    static char *f1 = "%%%%BoundingBox: %d %d %d %d\n";
    static char *f2 = "%%%%Pages: %d\n";

    fp1 = fopen(sfn, "rt");
    while (fgets(line, 256, fp1)) {
	if (strncmp(line, s1, n1) == 0)
	    nb = get_nums(line + n1, b, 4);
    	else if (strncmp(line, s2, n2) == 0)
	    np = get_nums(line + n2, p, 1);
    }
    fclose(fp1);

    if (nb == 4 && np == 1 && p[0] == 1) {
	strcpy(tfn, sfn);
        newext(tfn, ".~~~");
        fp1 = fopen(sfn, "rt");
        fp2 = fopen(tfn, "wt");
        while (fgets(line, 256, fp1)) {
    	    if (strncmp(line, s1, n1) == 0)
		fprintf(fp2, f1, b[0], b[1], b[2], b[3]);
    	    else if (strncmp(line, s2, n2) == 0)
		fprintf(fp2, f2, p[0]);
	    else
		fputs(line, fp2);
	}
        fclose(fp1);
	fclose(fp2);
	remove(sfn);
	rename(tfn, sfn);
    }
}

static UINT_PTR CALLBACK 
OFNHookProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
    if (prevfilt != ofn.nFilterIndex) {
        prevfilt = ofn.nFilterIndex;
        ofn.lpstrDefExt = dext[prevfilt];
    }
    return (0);
}


static void
save_plot(char *pfn)
{
    char           *av[4];
    int             ac = 0, i;
    
    strcpy(sfn, pfn);
    newext(sfn, "");
    
    // Fill out the OPENFILENAME struct
    ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
    ofn.lStructSize = sizeof( OPENFILENAME );
    ofn.hwndOwner = hWnd;
    ofn.hInstance = hInst;
    ofn.lpstrFile = sfn;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK | OFN_EXPLORER;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = prevfilt;
    ofn.lpstrTitle = "PLT Save As...";
    ofn.lpfnHook = OFNHookProc;
    ofn.lpstrDefExt = "";

    // Use the common dialog
    if (GetSaveFileName( &ofn )) { 
	i = ofn.nFilterIndex;
        if (i == mergfile) {
	    av[ac++] = "wplt";
	    av[ac++] = "-i";
	    av[ac++] = sfn;
	    av[ac++] = pfn;
	    tplt(ac, av);
        } else if (outfile(sfn)) {
	    orient(i == 1);
	    av[ac++] = "wplt";
	    av[ac++] = mode[i];
	    av[ac++] = pfn;
	    tplt(ac, av);
	    if (mode[i][2] == '5')
		eps_fix(sfn);
	}
        file_flag = 0;
	prevfilt = i;
    }
}

static void
copy_clip(char *pfn)
{

    if (OpenClipboard(NULL)) {
        EmptyClipboard();
        SetClipboardData(CF_BITMAP, hNBM);
	CloseClipboard();
    }
}

static void
batch_print()
{
    int i, n;
    char fn[MAX_MULTI], file_name[MAX_PATH];
    DWORD fa;
    static char *filter = "PLT Files\0" "*.plt\0"
                          "All Files\0" "*.*\0";

    if (win_multi_open("PLT BATCH PRINT Files", filter, fn)) {
	fa = GetFileAttributes(fn);
	if (fa) {
	    if (fa & FILE_ATTRIBUTE_DIRECTORY) {
		strcpy(file_name, fn);
		strcat(file_name, "\\");
		n = strlen(file_name);
		print_plot(file_name, 1);
		for (i = n; fn[i]; i++) {
		    strcpy(file_name + n, fn + i);
	            print_plot(file_name, 2);
		    i += strlen(fn + i);
		}
		print_plot(file_name, 3);
    	    } else {
		print_plot(fn, 0);
	    }
	}
    }
}


/****************************************************************************
 *  parse_command_line
 ****************************************************************************/

char **
parse_command_line(char *C, int *argc)
{
    char *s;
    int n = 0;
    static char *argv[80];

    s = _strdup(C);
    argv[n++] = "arg0";
    while (*s) {
	while (isspace(*s))
	    s++;
	if (*s)
	    argv[n++] = s++;
	while (*s && !isspace(*s))
	    s++;
	if (*s)
	    *s++ = 0;
    }
    *argc = n;
    return (argv);
}

/****************************************************************************
 *  WinMain
 ****************************************************************************/

int             WINAPI
WinMain(HINSTANCE I, HINSTANCE P, LPSTR C, int S)
{
    int     ac = 0, vflg = 0, f = 0, i;
    char  **av, fn[MAX_PATH], *editor;
    static char *pgm = "PLT";
    static char *filter = "PLT Files\0" "*.plt\0"
                          "All Files\0" "*.*\0";
    static char *lfn = "plt.log";

#ifdef mgw
    av = parse_command_line(C, &ac);
    av[0] = pgm;
#else
    ac = __argc;
    av = __argv;
#endif
    for (i = 1; i < ac && av[i][0] == '-'; i++) {
        if (av[i][1] == 'v') {
            vflg++;
        }
    }
    make_help_path(av[0]);
    editor = getenv("PLT_EDIT");
    if (editor == NULL)
	editor = "notepad";
    if (!win_init(I, pgm))
        return (1);
    if (vflg) {
        version();
        fatal(0);
        return (0);
    }
    if (ac > i) {
        strcpy(file_name, av[i]);
        if (noext(file_name))
            newext(file_name, ".plt");
	sound_enable(1);
    }
    do {
        if (f == OPEN_FILE) {
            if (win_open("PLT Files", filter, fn)) {
                if (_access(fn, 0) == 0) {
                    strcpy(file_name, fn);
                    av[1] = file_name;
                    ac = 2;
		    file_time = 0;
		    num_errs = 0;
		    set_page(0);
		    beg_pg();
                }
            } else if (ac > i) {
                strcpy(file_name, av[i]);
                if (noext(file_name))
                    newext(file_name, ".plt");
            }
	    lfn = inq_logfn();
	    if (_access(lfn, 0) == 0) {
                remove(lfn);
	    }
	    sound_enable(1);
	    zoom_level = 0;
	    num_errs = 0;
        } else if (f == REPLOT && *file_name) {
	    sound_enable(1);
	    set_page(0);
        } else if (f == UPDATE && *file_name) {
	    //num_errs = 0;
        } else if (f == SAVE_FILE && *file_name) {
            save_plot(file_name);
        } else if (f == EDIT_FILE) {
            sprintf(edit_cmnd, "%s %s", editor, file_name);
            WinExec(edit_cmnd, S);
        } else if (f == VIEW_LOG) {
	    lfn = inq_logfn();
	    if (_access(lfn, 0) == 0) {
                sprintf(edit_cmnd, "%s %s", editor, lfn);
                WinExec(edit_cmnd, S);
	    }
        } else if (f == PRINT_FILE && *file_name) {
            print_plot(file_name, 0);
        } else if (f == BATCH_PRINT) {
            batch_print();
        } else if (f == COPY_CLIP && *file_name) {
            copy_clip(file_name);
        }
        win_title(file_name);
	if (_access(file_name, 0) == 0)
	    file_time = win_file_time(file_name);
        tplt(ac, av);
        sound_enable(0);
        f = win_wait();
        sound_wait();
    } while (!win_done());
    win_quit();

    return (0);
}
