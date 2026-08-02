/*
 * 	setup - parses command line of plt and initializes program
 *
 *	Functions called:
 *		    alloar, clocktime, defflg, defpar, errout, fatal, filetime,
 *		    fprintf, lookup, opnout, page, resetout, setnpn, setpgp,
 *		    settxpr, sprintf, strcpy, strlen, version, clrvar
 *
 *	Called by:
 *		    main
 *
 */
#include <string.h>
#include <stdlib.h>
#include "plt.h"
#include "pltblk.h"
#include "cincl.h"
#include "fort.h"

int     lookup(char *);
int     inq_break(void);
void    alloar(void);
void    clrvar(void);
void    defflg(void);
void    defpar(void);
void    fatal(int);
void    setcpd(char *);
void    mac_clear(void);
void    pubclr(void);
void    version(void);

extern int firstf;

int
setup(int iecho, int idevice, int itxprec)
{
    char   *clocktime(void);
    char   *filetime(void);

    /* output device may require this info prior to opening */
    registerfn(misc_.filinm);
    set_title(misc_.filinm);
    set_creator("BTNRH PLT");
    set_date(clocktime());

    if (inq_break())
	return (0);
    if (!firstf) {
	page();
    } else {
	if (!opnout(idevice)) {	/* Open output */
	    sprintf(ctext_.msg, "plt: can't open device #%d\n", idevice);
	    fatal(2);
	    return (0);
	}
	if (misc_.slomod) {	/* added detail if "slow mode" is set */
	    setwidestroke(1);	/* use wide strokes */
	}
	alloar();		/* Allocate memory for the data arrays */
	firstf = 0;
    }

    clrvar();			/* Clear all user variables */
    defpar();			/* Set default parameter values */
    defflg();			/* Set default flag values */
    mac_clear();		/* clear macro definitions */
    pubclr();			/* clear put-back buffer */
    resetout();			/* Reset output routines */
    settxpr(itxprec);		/* Set text precision */
    if (misc_.pgprmt >= 0)
	setpgp(misc_.pgprmt);	/* Set page prompt */
    if (misc_.pgbord >= 0)
	setpgb(misc_.pgbord);	/* Set page border */
    if (misc_.numpen >= 0)
	setnpn(misc_.numpen);	/* Set number of pens */
/*
 *     Set the input and output logical unit numbers
 */
    cincl_.inclev = 0;		/* Set include level to 0 */
    cincl_.inc_file[0] = misc_.lunin;	/* Set include file pointer */
    param_.echo = (float) iecho;
    misc_.device = idevice;
    misc_.i_pltype = lookup("PLTY");
    misc_.i_ticdir = lookup("TICD");
    misc_.i_mhalign = lookup("MHAL");
    misc_.i_mvalign = lookup("MVAL");
/*
 *     Get time and date for the "header"
 */
    strcpy(misc_.hdate, filetime());	/* get file time, if possible */
    if (strlen(misc_.hdate) == 0) {	/* if no file time */
	strcpy(misc_.hdate, clocktime());	/* get current time of day */
    }
#ifdef DEBUG
    if (flags_.dbflg) {
	version();
	errout(ctext_.msg);
	sprintf(ctext_.msg,
	    " setup: input file = %s\n", misc_.filinm);
	errout(ctext_.msg);
	sprintf(ctext_.msg,
	    " setup: echo=%d, model=%d\n", iecho, idevice);
	errout(ctext_.msg);
    }
#endif
    setcpd(misc_.filinm);
    return (1);
}

void
setcpd(char *fn)
{
    char *s, *p;

    strncpy(misc_.curpltdir, fn, NAMESIZE);
    p = misc_.curpltdir;
    s = p + strlen(p) - 1;
    while (s > p && *s != '/' && *s != '\\' && *s != ':')
	*s-- = '\0';
    if (s == p)
	*s = '\0';
}

/* alloar - allocate memory for text and data arrays */

extern int maxnp;
extern int ndatp;
extern float *xdat, *ydat, *zdat;

#define MINDATA 255
#define MAXDATA 123456

void
alloar(void)
{
    ctext_.xfmt = (char *) calloc(1, 1);
    ctext_.yfmt = (char *) calloc(1, 1);
    ctext_.tfmt = (char *) calloc(1, 1);
    ctext_.rfmt = (char *) calloc(1, 1);

    ctext_.xlabel = (char *) calloc(1, 1);
    ctext_.ylabel = (char *) calloc(1, 1);
    ctext_.tlabel = (char *) calloc(1, 1);
    ctext_.rlabel = (char *) calloc(1, 1);
    ctext_.xannot = (char *) calloc(1, 1);
    ctext_.yannot = (char *) calloc(1, 1);
    ctext_.tannot = (char *) calloc(1, 1);
    ctext_.rannot = (char *) calloc(1, 1);

    ctext_.xdata = (char *) calloc(1, 1);
    ctext_.ydata = (char *) calloc(1, 1);
    ctext_.zdata = (char *) calloc(1, 1);
    ctext_.select = (char *) calloc(1, 1);

    ndatp = 0;
    maxnp = MINDATA;
    xdat = (float *) calloc(maxnp, sizeof(float));
    ydat = (float *) calloc(maxnp, sizeof(float));
    zdat = (float *) calloc(maxnp, sizeof(float));
    if (xdat == NULL || ydat == NULL || zdat == NULL) {
	sprintf(ctext_.msg, "ERROR: Can't allocate memory for data arrays.\n");
	fatal(3);
    }
}

int
mordat(void)
{
    int     i;
    float  *xd, *yd, *zd;
    unsigned int n;

    if ((n = maxnp * 2) > MAXDATA)
	return (0);
    xd = (float *) calloc(n, sizeof(float));
    yd = (float *) calloc(n, sizeof(float));
    zd = (float *) calloc(n, sizeof(float));
    if (xd == NULL || yd == NULL || zd == NULL) {	/* not enough memory */
	if (xd != NULL)
	    free(xd);
	if (yd != NULL)
	    free(yd);
	if (zd != NULL)
	    free(zd);
	return (0);
    }
    for (i = 0; i < ndatp; i++) {
	xd[i] = xdat[i];
	yd[i] = ydat[i];
	zd[i] = zdat[i];
    }
    free(xdat);
    free(ydat);
    free(zdat);
    maxnp = n;
    xdat = xd;
    ydat = yd;
    zdat = zd;
    return (1);
}

int
gtdvnm(void)
{
    return (misc_.device);
}
