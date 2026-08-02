/* indata.c */
/*
 *
 * 	int indata (instr)
 *
 *
 *	DATE OF  FIRST  VERSION:	 4-Apr-80
 * 	DATE OF CURRENT VERSION:	30-Aug-88
 *
 *	PURPOSE:	PLT routine to decode one data value.
 *
 *	USAGE:	istat = indata(instr)
 *
 *		      instr  -- char array that holds the first input line
 *
 *		      ISTAT  -- Exit status:
 *				  -1 = Error in datum decode
 *				   0 = Data entered OK
 *				   1 = Data entered OK, but buffer is
 *					full with more data to come
 *
 *		 * denotes single datum, entered from keyboard
 *		** denotes multiple data, entered from keyboard
 *
 *	Functions called:
 *			errout, getdat, get_tty, indx, strcpy, strlen
 *
 *	Called by:
 *			proces
 *
 *	PROGRAMMER:	Eric Javel / Matt Prucka
 *
 **********************************************************************
 */

#include <string.h>
#include <math.h>
#include "plt.h"
#include "pltblk.h"
#include "fort.h"

#define NCOL 99

extern int maxnp;
extern int ndatp;
extern float *xdat, *ydat, *zdat;

double	selecval(void);
double	xdataval(void);
double	ydataval(void);
double	zdataval(void);
int     getdat(char *, double *, int);
int     getnum(char *, double *, char *);
int     get_tty(char *, char *, int);
int     mordat(void);

static double datval(char *, char *);
static double datcol[NCOL];

int
indata(char *instr)
{
    int     i, ndrep;
    long    multi, keybd;

/*
 *
 * -----------------------------------------------------------------------
 *
 *     See if user wants data value input from the keyboard
 */
    multi = FALSE;
    keybd = FALSE;
    if (instr[0] == '*') {
	if (!misc_.in_tt) {
	    errout("error: keyboard DATA input requested\n");
	    return (0);
	}
	keybd = TRUE;
	if (instr[1] == '*') {	/* Multiple data, maybe */
	    multi = TRUE;
	    errout(" Input multiple data. Terminate with blank line ...\n\n");
	}
    }
    do {			/* loop on multiple data from keyboard */
/*
    Test for buffer full
 */
        ndrep = (int) param_.ndata;
        while (ndatp + ndrep > maxnp) {
            if (mordat()== 0) {
                if (multi) {
                    strcpy(instr, "**");	/* Restore multiple data flag */
                    errout(" *** Please wait while data are plotted ***\n");
                 }
                return (1);
            }
        }

/*
	Get the line from the keyboard if we need to do that
 */
        if (keybd) {
            (void) get_tty(" Enter X,Y [,Z]: ", instr, 128);
            if (strlen(instr) == 0) {		/* Blank line, so ignore input */
                return (0);
            }
        }
/*
	Decode the data line
 */
        for (i = 0; i < NCOL; i++)
            datcol[i] = 0.0;			/* set default data values */
        if (getdat(instr, datcol, NCOL) < NCOL) {
            sprintf(ctext_.msg, "error in data value: %s\n", instr);
            errout(ctext_.msg);
        }
/*
	Save data values in xdat, ydat, zdat arrays
 */
        while (ndrep-- > 0) {
            i = ndatp++;
            xdat[i] = (float) xdataval();
            ydat[i] = (float) ydataval();
            zdat[i] = (float) zdataval();
            if (isnan(xdat[i]) || isnan(ydat[i]) || isnan(zdat[i])) {
                ndatp--;
            }
            if (selecval() == 0) {
                ndatp--;
            }
        }

    } while (multi);		/* Get more if mult. data from keyboard */
    return (0);
}

double
colval(int i)
{
    return ((i < 1) ? ndatp : (i > NCOL) ? 0 : datcol[i - 1]);
}

static double
datval(char *dstr, char *name)
{
    double  tmp = 0;
    static int rdep = 0;
    static int msgcnt = 0;

    if (rdep++ > 3) {
        sprintf(ctext_.msg, "recursion error in %s string: %s\n", name, dstr);
        if (msgcnt++ < 20)
	    errout(ctext_.msg);
    } else if (getnum(dstr, &tmp, "") < 0) {
        sprintf(ctext_.msg, "error in %s string: %s\n", name, dstr);
        if (msgcnt++ < 20)
	    errout(ctext_.msg);
    }
    rdep--;
    return (tmp);
}

double
xdataval(void)
{
    double  tmp = 0;

    if (*ctext_.xdata < ' ') {
        tmp = datcol[0] * param_.xgain + param_.xofst;
    } else {
        tmp = datval(ctext_.xdata, "xdata");
    }
    return (tmp);
}

double
ydataval(void)
{
    double  tmp = 0;

    if (*ctext_.ydata < ' ') {
        tmp = datcol[1] * param_.ygain + param_.yofst;
    } else {
        tmp = datval(ctext_.ydata, "ydata");
    }
    return (tmp);
}

double
zdataval(void)
{
    double  tmp = 0;

    if (*ctext_.zdata < ' ') {
        tmp = datcol[2] * param_.zgain + param_.zofst;
    } else {
        tmp = datval(ctext_.zdata, "zdata");
    }
    return (tmp);
}

double
selecval(void)
{
    double  tmp = 0;

    if (*ctext_.select < ' ') {
        tmp = 1;
    } else {
        tmp = datval(ctext_.select, "select");
    }
    return (tmp);
}

void
swap_datcol(int n, double *a)
{
    int     i;
    double  t;

    for (i = 0; i < n; i++) {
        t = a[i];
        a[i] = datcol[i];
        datcol[i] = t;
    }
}
