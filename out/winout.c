/* winout.c - WinMain for libout with win32 */

#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <windowsx.h>
#include <winspool.h>
#include "winout.h"

#ifndef WIN32
#define _strdup strdup
#endif

int     win_init(HINSTANCE, char *);
void    print_end();
void    print_start();
void    tstout(int, char **);
void    win_page();
HDC     GetPrinterDC(void);

static int file_flag = 0;

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

int WINAPI
WinMain(HINSTANCE I, HINSTANCE P, LPSTR C, int S)
{
    char **av;
    int ac, f = 0;
    static char *pgm = "TSTOUT";

#ifdef mgw
    av = parse_command_line(C, &ac);
    av[0] = pgm;
#else
    ac = __argc;
    av = __argv;
#endif
    if (!win_init(I, pgm))
        return (1);
    if (ac > 1) {
        tstout(ac, av);
    } else {
        do {
	    if (f == PRINT_FILE)
	        print_start();
	    tstout(ac, av);
	    f = file_flag;
	} while (!win_done());
    }
    win_quit();
    return (0);
}

/****************************************************************************
 *  WIN32 FUNCTIONS:
 ****************************************************************************/
static void msg_loop();

static int loop_flag = 0;
static int exit_flag = 0;
static int resize_flag = 1;

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

void
beg_pg()
{
    RECT r;

    GetClientRect(hWnd, &r);
    InvalidateRect(hWnd, &r, FALSE);
}

void
end_pg()
{
    UpdateWindow(hWnd);
    if (!exit_flag && !file_flag) {
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

/*
 *  OutMenu() - called on WM_COMMAND messages
 */
void
OutMenu(HWND hWindow, INT idval)
{
    int cx, cy, wx, wy;
    RECT r;

    switch (idval) {

    case ENTER:
        loop_flag--;
        break;

    case EXIT:
        PostMessage(hWindow, WM_CLOSE, 0, 0L);
        break;

    case PRINT_FILE:
    case REPLOT:
        file_flag = idval;
        loop_flag = 0;
        break;

    case SET_SIZE:
        SetWindowPos(hWindow, HWND_TOP, 0, 0, xwin, ywin, SWP_NOMOVE);
        break;

    case SET_ASPECT:
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
        SetWindowPos(hWindow, HWND_TOP, 0, 0, wx, wy, SWP_NOMOVE);
        file_flag = REPLOT;
        loop_flag = 0;
        break;

    case MN_HELP_ABOUT:
        DialogBox(hInst, MAKEINTRESOURCE(ABOUTBOX), hWindow,
            (DLGPROC)lpAbout );
        break;
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
    PAINTSTRUCT   ps;
    POINT         curpoint;

    switch (message) {

    case WM_COMMAND:
        OutMenu(hWindow, GET_WM_COMMAND_ID(wParam, lParam));
        break;

    case WM_SIZE:
        UpdateWindow(hWindow);
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
        if (wParam && (!HIWORD(lParam)))
            SetFocus(hWindow);
            break;

    case WM_PAINT:
        BeginPaint(hWindow, (LPPAINTSTRUCT)&ps);
        out_paint(hWindow, ps);
        EndPaint(hWindow, (LPPAINTSTRUCT)&ps);
        break;

    case WM_DESTROY:
        hWnd = 0;
        exit_flag = 1;
        PostQuitMessage(0);
        break;

    default:
        return(DefWindowProc(hWindow, message, wParam, lParam));
        break;
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
        OutClass.style         = 0;
        OutClass.hInstance     = hInstance;
        OutClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(WINOUT_ICON));
        OutClass.hCursor = curCross;
        OutClass.hbrBackground = NULL;
        OutClass.lpszMenuName  = MAKEINTRESOURCE(WINOUT);
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

int
win_done()
{
    print_end();
    if (!exit_flag && !file_flag) {
        loop_flag = -1;
        msg_loop();
    }
    if (file_flag) {
        create_DCDrw();
        file_flag = 0;
    }
    return (exit_flag);
}

int
win_file()
{
    return (file_flag);
}


void
win_mesg(char *caption, char *msg)
{
    (void) MessageBox(hWnd, msg, caption, MB_OK);
}

void
win_title(char *s)
{
    if (hWnd)
        SetWindowText(hWnd, s);
}

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
    strcpy(fn, filter);
    ofn.lpstrFile = fn;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;
    ofn.lpstrFilter = filter;
    ofn.lpstrTitle = title;

    // Use the common dialog
    return GetOpenFileName( &ofn );
}

/**********************************************************************/

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

    return 0;
}

void
print_start()
{
    int x, y;
    static DOCINFO  di = {sizeof(DOCINFO), "WINOUT", NULL};

    hdcPrint = GetPrinterDC();
    if (hdcPrint) {
        x = (int) (GetDeviceCaps(hdcPrint, LOGPIXELSX) * 8.0 + 0.5);
        y = (int) (GetDeviceCaps(hdcPrint, LOGPIXELSY) * 10.5 + 0.5);
        win_DC(hdcPrint, 0, y, x, 0);
        win_page(pr_beg_pg, pr_end_pg);
        (void) StartDoc(hdcPrint, &di);
    }
}

void
print_end()
{
    if (hdcPrint) {
        EndDoc(hdcPrint);
        DeleteDC(hdcPrint);
        hdcPrint = 0;
        file_flag = REPLOT;
        win_page(beg_pg, end_pg);
    }
}
