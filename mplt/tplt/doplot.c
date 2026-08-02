/* doplot.c */
/*
 * doplot - plot data and frame
 */
#include "plt.h"
#include "pltblk.h"
#include "fort.h"

#define CX1 param_.xllc
#define CY1 param_.yllc
#define CX2 param_.xllc+param_.xlen
#define CY2 param_.yllc+param_.ylen
#define DX1 xpos(param_.xmin)
#define DY1 ypos(param_.ymin)
#define DX2 xpos(param_.xmax)
#define DY2 ypos(param_.ymax)
#define WX1 param_.wxllc
#define WY1 param_.wyllc
#define WX2 param_.wxllc+param_.wxlen
#define WY2 param_.wyllc+param_.wylen
#define VX1 param_.vxllc/inch_unit
#define VY1 param_.vyllc/inch_unit
#define VGN param_.magnify/inch_unit
#define VRO param_.rotate

double  xpos(double);
double  ypos(double);
int     inq_break(void);
void    axes1(void);
void    axes2(void);
void    axes3(void);
void    axes4(void);
void    axes5(void);
void    dattim(void);
void    histo(int, float *, float *);
void    cumplt(int);
void    frqplt(int);
void    linplt(int);
void    polygon(int, float *, float *);
void    rect(int, float *, float *, float *);
void    cntgrect(int, float *, float *, float *);
void    setaxpar(void);
void    setsiz(void);
void    wave(int, float *, float *);
void    set_page(int);
void	reset_frame(void);
void	set_frame(struct axes);

extern double inch_unit;
extern int ndatp;
extern float *xdat, *ydat, *zdat;

float   xanlen = 0, yanlen = 0, ranlen = 0, tanlen = 0;

int
init_frame(void)
{
    setsiz();
    setwin(WX1, WY1, WX2, WY2);
    setvup(VX1, VY1, VGN, VRO);
    setclp(WX1, WY1, WX2, WY2);		// Don't clip axes or messages

    if (flags_.newpag) {		// See if this is a new page
        if (misc_.pltyet)
	    page();
	misc_.pltyet = FALSE;
	misc_.hdryet = FALSE;
	flags_.newpag = FALSE;
        flags_.axflg = TRUE;
        reset_frame();
    }
    if (flags_.rngflg || flags_.axflg)	{ // Set axis parameters 
	setaxpar();
	set_frame(axes_);
    }
    if (inq_break()) {
	return (1);
    }
    set_page(misc_.inow);

    return (0);
}

void
doplot(int nkeep)
{
    if (init_frame())
	return;
    if ((misc_.ifrom <= misc_.inow) && (misc_.inow <= misc_.ito)) {
	misc_.pltyet = TRUE;
	/* Do the header, maybe */
	if ((int) param_.header && !misc_.hdryet) {
	    dattim();
	    misc_.hdryet = TRUE;
	}
	/* Draw the axes, if that has not been done yet. */
	if (flags_.rngflg || flags_.axflg) {
	    if (flags_.axflg) {
		axes1();		/* Axis lines */
		flags_.rngflg = TRUE;
	    }
	    if (flags_.rngflg) {
		axes2();		/* Tick marks */
		axes3();		/* Grid lines */
		axes4();		/* Annotations */
		flags_.rngflg = FALSE;
	    }
	    if (flags_.axflg) {
		axes5();		/* Axis labels */
		flags_.axflg = FALSE;
	    }
	}
	if (ndatp > 0) {	/* Draw the data, if data exist */
	    if (param_.clip == 2) {
		setclp(DX1, DY1, DX2, DY2);
	    } else if (param_.clip != 0.) {
		setclp(CX1, CY1, CX2, CY2);
	    }
	    switch ((int) param_.pltype) {
	    case 1:
	    case 2:
	    case 3:
		linplt(ndatp);
		break;
	    case 4:
		histo(ndatp, xdat, ydat);
		break;
	    case 5:
		rect(ndatp, xdat, ydat, zdat);
		break;
	    case 6:
		polygon(ndatp, xdat, ydat);
		break;
	    case 7:
		wave(ndatp, xdat, ydat);
		break;
	    case 8:
		cntgrect(ndatp, xdat, ydat, zdat);
		break;
	    case 9:
		cumplt(ndatp);
		break;
	    case 10:
		frqplt(ndatp);
		break;
	    }
	}
	updout();		/* flush the output */
    }
    misc_.npkept = ndatp = nkeep;	/* clear data list ? */
}
