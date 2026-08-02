/*
 * output - output primitives
 */
#include <string.h>
#include "output.h"
#include "device.h"
#include "fort.h"
#include "font.h"
#include "clip.h"
#include "copa.h"
#include "cview.h"
#include "cwind.h"
#include "dbind.h"

#ifndef _MSC_VER
#include <unistd.h>
#define _fileno fileno
#define _isatty isatty
#endif
#ifndef WIN32
#ifndef IPHONE
#define INQ_BREAK
#endif
#endif

#define MXFN    256
#define MAXERR	1000

/* define globals */

float   x_page = 10.5, y_page = 8.0;
short   o_page = 0;

struct cclip cclip_ = {0};
struct cwind cwind_ = {0};
struct cview cview_ = {0};
struct dev dev_ = {0};
struct copa copa_ = {0};

extern FONT stdfont_, altfont_, symfont_;
FONT   *curfont[] = {&stdfont_, &altfont_, &symfont_};
FONT   *deffont[] = {&stdfont_, &altfont_, &symfont_};

int     (*d_open) (void) = NULL;
void    (*d_close) (void) = NULL;
void    (*d_trns) (double, double, int) = NULL;
void    (*d_curv) (float *, float *) = NULL;
void    (*d_page) (void) = NULL;
void    (*d_pnwd) (double) = NULL;
void    (*d_fill) (int, float *, float *, int, int) = NULL;
void    (*d_text) (double, double, char *) = NULL;
void    (*d_qtxx) (double, double, char *, TXX *) = NULL;
void    (*d_setpal) (void) = NULL;
void    (*d_clip) (void) = NULL;

void    dvpnwd(double);
void    dvsetpal(void);
void    dvclip(void);

RGB_COLOR ct16[16] = {
    {0, 0, 0},			/* black      */
    {0, 0, 667},		/* blue       */
    {0, 667, 0},		/* green      */
    {0, 667, 667},		/* cyan       */
    {667, 0, 0},		/* red        */
    {667, 0, 667},		/* magenta    */
    {667, 333, 0},		/* brown      */
    {667, 667, 667},		/* lt grey    */
    {333, 333, 333},		/* dk grey    */
    {333, 333, 1000},		/* lt blue    */
    {333, 1000, 333},		/* lt green   */
    {333, 1000, 1000},		/* lt cyan    */
    {1000, 333, 333},		/* lt red     */
    {1000, 333, 1000},		/* lt magenta */
    {1000, 1000, 333},		/* yellow     */
    {1000, 1000, 1000},		/* white      */
};

RGB_COLOR *ct = ct16;
int     ct_min = 0, ct_max = 1000;

static char regfn[MXFN] = {0};
static char logfn[MXFN] = {0};
static char fnflg = 0;

int
binddev(int device)
{
    if (device < 0 || device > NDEV || dev_.isopen != 0)
	return (0);

/* Each device driver must bind at least four functions:
 *
 *     d_open   - called once, prior to any output
 *     d_trns   - implements move and draw functions
 *     d_page   - signals start of a new page
 *     d_close  - called last, before program exits
 *
 * The device driver may also choose to overide the area fill function
 * or the text functions.  These functions have default stroke versions.
 */
    d_pnwd = dvpnwd;	/* set pen width */
    d_fill = skfill;	/* fill an area */
    d_text = sktext;	/* output a text string */
    d_qtxx = skqtxx;	/* calculate extent of a text string */
    d_setpal = dvsetpal;/* set color palette */
    d_clip = dvclip;	/* set clip boundary */
    d_curv = curve;	/* set clip boundary */

    DEV = device;
    d_bind[DEV]();

    return (1);
}

static void
make_log_path(char *fn)
{
    char    *e;

    /* extract directory from plt file name */
    strcpy(logfn, regfn);
    for (e = &logfn[strlen(logfn) - 1]; e > logfn; e--) {
	if (*e == '/' || *e == '\\') {
	    e++;
	    break;
	}
    }
    *e = '\0';
    /* append log file name */
    strcat(logfn, fn);
}

void
errout(char *msg)
{
    if (copa_.nerrs == 0 && copa_.logfile == NULL) {
        make_log_path("plt.log");
        copa_.lfname = logfn;
	remove(logfn);
        copa_.logfile = fopen(copa_.lfname, "w");
    }
    if (copa_.logfile != NULL) {
        if (copa_.nerrs == 0) {
            fprintf(copa_.logfile, "PLT error log - %s\n", copa_.date);
        }
        if (fnflg && regfn[0]) {
            fprintf(copa_.logfile, "\n======== file %s ========\n", regfn);
            fnflg = 0;
        }
	fputs(msg, copa_.logfile);
    }
    copa_.nerrs++;
    if (copa_.nerrs > MAXERR) {
        dev_.prompt = 0;
        if (copa_.logfile != NULL) {
            fprintf(copa_.logfile, ">>> MORE THAN %d ERRORS! <<<\n", MAXERR);
        }
        clsout();
        fprintf(stderr, "\007");
        exit(1);
    }
}

int
errcnt(void)
{
    return (copa_.nerrs);
}

void
registerfn(char *fn)
{
    strncpy(regfn, fn, MXFN);
    logfn[0] = '\0';
    fnflg = 1;
}

void
orient(int o)
{
    o_page = (short) o;
}

/* "do nothing" function for unused device driver functions */

void dvpnwd(double pw) {}
void dvsetpal(void) {}
void dvclip(void) {}

/****************************************************************************/

#include <math.h>

#define MAX_FRM	64

struct axes {
    short   logx, logy, logt, logr;
    short   xaxis, yaxis, raxis, taxis;
    float   axx, ayy, att, arr;
    float   axo, ayo, ato, aro;
    float   rxmin, rxmax, rxlen, rxllc;
    float   rymin, rymax, rylen, ryllc;
};

static int	    num_frms = 0;
static struct axes  frm[MAX_FRM];
static struct cview vup[MAX_FRM];

void
reset_frame(void)
{
    num_frms = 0;
}

void
set_frame(struct axes a)
{
    int i;

    if (num_frms < MAX_FRM) {
        for (i = 0; i < num_frms; i++) {
	    if (a.rxllc == frm[i].rxllc 
		&& a.ryllc == frm[i].ryllc
		&& a.rxlen == frm[i].rxlen
		&& a.rylen == frm[i].rylen
		&& a.axo == frm[i].axo
		&& a.ayo == frm[i].ayo
		&& a.axx == frm[i].axx
		&& a.ayy == frm[i].ayy
		&& a.logx == frm[i].logx
		&& a.logy == frm[i].logy) {
		break;
	    }
	}
	if (i == num_frms) {
	    vup[num_frms] = cview_;
	    frm[num_frms++] = a;
	}
    }
}

static void
view_trns(double *x, double *y, double *xx, double *yy, struct cview v)
{
    *xx = (*x - v.vx1) / v.vgn;
    *yy = (*y - v.vy1) / v.vgn;
}

int
inq_frame(double *x, double *y, int m)
{
    double xx = 0, yy = 0;
    int i = 0;

    if (m) {
        for (i = num_frms - 1; i >= 0; i--) {
	    view_trns(x, y, &xx, &yy, vup[i]);
	    if (xx >= frm[i].rxllc 
		&& yy >= frm[i].ryllc
		&& xx <= frm[i].rxllc + frm[i].rxlen
		&& yy <= frm[i].ryllc + frm[i].rylen) {
		break;
	    }
	}
    	if (num_frms == 1) {
	    i = 0;
	}
    	if (i < 0) {
	    m = 0;
	}
    }
    if (m == 0) {
        view_trns(x, y, x, y, cview_);
    } else if (m == 1) {
        *x = xx - frm[i].rxllc;
        *y = yy - frm[i].ryllc;
    } else if (m == 2) {
        *x = (xx - frm[i].axo) / frm[i].axx;
        if (frm[i].logx) {
	    *x = pow(10.0, *x);
	}
	*y = (yy - frm[i].ayo) / frm[i].ayy;
	if (frm[i].logy) {
	    *y = pow(10.0, *y);
	}
    }

    return (m);
}

/****************************************************************************/

static int start_page = 1;  /* default start page = 1 */

void
set_page(int p)
{
    start_page = p;
}

int
inq_page(void)
{
    return (start_page);
}

char *
inq_logfn(void)
{
    return (logfn);
}

#ifdef INQ_BREAK

int
inq_break(void)
{
    return (0);
}

#endif

