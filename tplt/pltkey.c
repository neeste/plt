/*
 *  pltkey(char *instr)
 *
 *
 * 	DATE OF  FIRST  VERSION:	 1-Apr-80
 * 	DATE OF CURRENT VERSION:	10-Oct-90       STN
 *
 * 	PURPOSE:	PLT routine to control activities associated
 * 			with keywords.
 *
 * 	USAGE:	pltkey (instr)
 *
 * 	where:
 * 			instr  = Command line entered from LUNIN
 *
 * 	Called by:
 *		        proces
 *
 * 	Functions called:
 *			bindat, defflg, defpar, doplot, errout, iscan, opnfnt,
 *			opninc, resetout, sprintf, updout, wrdatl, wrprml
 *
 *
 *	PROGRAMMER:	Mike Collins / Eric Javel / Matt Prucka
 *
 **************************************************************
 */
#include "plt.h"
#include "pltblk.h"
#include "fort.h"

#define ALTF	1
#define BDAT	2
#define CALL	3
#define DATA	4
#define DATL	5
#define FINI	6
#define INCL	7
#define KEEP	8
#define NEWF	9
#define NEWP	10
#define NEWR	11
#define PLOT	12
#define PRML	13
#define RESE	14
#define STDF	15
#define SYMF	16
#define VARL	17
#define PALE	18
#define PLAY	19
#define EXIT	20
#define STAT	21
#define FILT	22
#define UWPH	23
#define SAVE	24
#define LOAD	25

int     iscan(char *[], int, char *);
int     mordat(void);
void    bindat(char *);
void    defflg(void);
void    defpar(void);
void    doplot(int);
void    opnfnt(char *, int);
void    opninc(char *);
void    wrprml(void);
void    wrdatl(int, float *, float *, float *, char *);
void    wrvarl(void);
void    playdata(void);
void    cstat(void);
void    datfilt(void);
void    unwrap_phase(void);
void    setpalette(int, float *, float *, float *);
void    save_data(void);
void    load_data(void);

extern int ndatp, nsavp, maxnp;
extern float *xdat, *ydat, *zdat;
extern float *xsav, *ysav, *zsav;

static char *keywrd[]= {				/* key words */
    "ALTF", "BDAT", "CALL", "DATA", "DATL", "FINI", "INCL", "KEEP", "NEWF",
    "NEWP", "NEWR", "PLOT", "PRML", "RESE", "STDF", "SYMF", "VARL", "PALE",
    "PLAY", "EXIT", "STAT", "FILT", "UWPH", "SAVE", "LOAD",
};
static int nkwrd = sizeof(keywrd) / sizeof(char *);	/* number of key words */

void
pltkey(char *pinstr)
{
    int     key;

    key = iscan(keywrd, nkwrd, pinstr);

#ifdef DEBUG
    if (flags_.dbflg) {
	sprintf(ctext_.msg, "pltkey: key = %d\n", key);
	errout(ctext_.msg);
    }
#endif

    switch (key) {
    case PLOT:			/* PLOT */
	doplot(0);
	break;
    case DATL:			/* DATALIST  */
	wrdatl(ndatp, xdat, ydat, zdat, pinstr);
	break;
    case KEEP:			/* KEEP */
	doplot(ndatp);
	break;
    case DATA:			/* DATA entered */
	ndatp = 0;
	misc_.npkept = 0;
	break;
    case FINI:			/* FINISH entered */
	flags_.finish = TRUE;
	break;
    case NEWP:			/* NEWPAGE entered */
        if (ndatp > 0 || (misc_.pltyet && !misc_.hdryet)) {
	    doplot(0);
	}
 	if (!flags_.newpag) {
	    flags_.newpag = TRUE;
	    misc_.inow++;
	}
	break;
    case NEWF:			/* NEWFRAME entered */
	if (!flags_.axflg) {
	    flags_.axflg = TRUE;
	}
	break;
    case NEWR:			/* NEWRANGE entered */
	if (!flags_.rngflg) {
	    flags_.rngflg = TRUE;
	}
	break;
    case PRML:			/* PRMLIST  */
	wrprml();
	break;
    case INCL:
    case CALL:			/* CALL or INCLUDE */
	opninc(pinstr);
	break;
    case RESE:			/* RESET */
	defpar();
	resetout();
	ndatp = 0;
	misc_.npkept = 0;
	break;
    case BDAT:			/* BDATA */
	bindat(pinstr);
	break;
    case STDF:			/* STDFNT */
	opnfnt(pinstr, 0);
	break;
    case ALTF:			/* ALTFNT */
	opnfnt(pinstr, 1);
	break;
    case SYMF:			/* SYMFNT */
	opnfnt(pinstr, 2);
	break;
    case VARL:			/* VARLIST  */
	wrvarl();
	break;
    case PALE:			/* PALETTE  */
	setpalette(ndatp, xdat, ydat, zdat);
	ndatp = 0;		/* clear the data list */
	break;
    case PLAY:			/* PLAY  */
	playdata();
	ndatp = 0;		/* clear the data list */
	break;
    case EXIT:			/* EXIT  */
	flags_.exit = TRUE;
	break;
    case STAT:			/* STAT  */
	cstat();
	break;
    case FILT:			/* FILT  */
	datfilt();
	break;
    case UWPH:			/* UWPH  */
	unwrap_phase();
	break;
    case SAVE:			/* SAVE  */
	save_data();
	break;
    case LOAD:			/* LOAD  */
	load_data();
	break;
    default:
	sprintf(ctext_.msg, "'%s' is not a valid keyword\n", pinstr);
	errout(ctext_.msg);
    }
}

void
datfilt(void)
{
    int     i, j;

    j = 0;
    for (i = 0; i < ndatp; i++) {
	if (xdat[i] >= param_.xmin && xdat[i] <= param_.xmax &&
	    ydat[i] >= param_.ymin && ydat[i] <= param_.ymax) {
	    if (j < i) {
		xdat[j] = xdat[i];
		ydat[j] = ydat[i];
		zdat[j] = zdat[i];
	    }
	    j++;
	}
    }
    ndatp = j;
}

void
unwrap_phase(void)
{
    float   c, hc;
    int     i;

    c = 1;
    hc = c / 2;
    for (i = 1; i < ndatp; i++) {
	while (ydat[i] - ydat[i - 1] < -hc)
	    ydat[i] += c;
	while (ydat[i] - ydat[i - 1] > hc)
	    ydat[i] -= c;
    }
}

void
save_data(void)
{
    int     i;

    if (xsav) {
	free(xsav);
    }
    if (ysav) {
	free(ysav);
    }
    if (zsav) {
	free(zsav);
    }
    xsav = (float *) calloc(ndatp, sizeof(float));
    ysav = (float *) calloc(ndatp, sizeof(float));
    zsav = (float *) calloc(ndatp, sizeof(float));
    for (i = 0; i < ndatp; i++) {
	xsav[i] = xdat[i];
	ysav[i] = ydat[i];
	zsav[i] = zdat[i];
    }
    nsavp = ndatp;
    ndatp = 0;
}

void
load_data(void)
{
    int     i;

    for (i = 0; i < nsavp; i++) {
	if (ndatp >= maxnp)
	    if (!mordat())
		break;
	xdat[ndatp] = xsav[i];
	ydat[ndatp] = ysav[i];
	zdat[ndatp] = zsav[i];
	ndatp++;
    }
}

void
playdata(void)
{
    int i;

    for (i = 0; i < ndatp; i++) {
	sound_play(xdat[i], ydat[i]);
    }
}

