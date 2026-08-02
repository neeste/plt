/* axanno.c
 *
 *	PURPOSE: PLT routine to plot axis annotations.
 *
 *	USAGE:
 *	 annoln = AXANNO (XFLAG,VMIN,VMAX,CYC,VINT,SKP,FMT,ANNPOS)
 *
 *			where
 *
 *			  XFLAG = Values:
 *				   1 for bottom axis
 *				   0 for left axis
 *				   1 for top axis
 *				   0 for right axis
 *			  VMIN = Minimum axis value
 *			  VMAX = Maximum axis value
 *			  VCYC = No. of cycles (0 if linear plot)
 *			  VINT = Number of major intervals on this axis
 *			   SKP = Annotation skip factor
 *			   FMT = Annotation format (ASCIZ string)
 *			ANNPOS = Position of the annotation
 *
 *	Programmer:	E. Javel / M. Prucka

 ********************************************************************
 */
#include <math.h>
#include "fort.h"
#include "plt.h"
#include "pltblk.h"

int     getnum(char *, double *, char *);
int     ifmt(char *);
int     indx(char *, int);
double  numb2(double, double, double, int);
double  lablen(char *);
double  xpos(double);
double  ypos(double);
double  tpos(double);
double  rpos(double);
void    label(double, double, char *);

double
axanno(int yflag, double vmin, double vmax, double vcyc, double vint, double skp,
       char *pfmt, double annpos)
{
    double  a, b, c, val;
    double  x, y, annoln, numlen, logbase = 10;
    double  x1, x2, y1, y2;
    int     jfmt, kfmt, linax;
    int     z, nskip, iskip, in;
/*
 *
 *----------------------------------------------------------------------
 *
 * Initialize
 */
    linax = (vcyc == 0.0);	/* Linear axis indicator */
    jfmt = ifmt(pfmt);		/* Format indicator */
    annoln = 0.0;
    if (linax) {
	if (vint < 1.0)		/* A linear axis with no intervals */
	    return (0.0);	/* will have no annotations. */
	a = vmax - vmin;
	b = floor(vint);
	c = a / b;
	val = vmin;
    } else {
	if (vint == -2.0) {
	    logbase = 2.0;
	    a = amin1(vmin, vmax);
	    b = 0.0;
	    c = log10(amax1(vmin, vmax) / a) / log10(logbase);
	    val = a;
	} else {
	    logbase = 10.0;
	    a = 1.0;
	    b = log10(amin1(vmin, vmax));
	    c = log10(amax1(vmin, vmax));
	    val = pow(logbase, b);
	}
    }
    x1 = axes_.rxllc - 0.001;
    y1 = axes_.ryllc - 0.001;
    x2 = axes_.rxllc + axes_.rxlen + 0.001;
    y2 = axes_.ryllc + axes_.rylen + 0.001;
    in = 0;

    /* Annotate until we run out of intervals (or do 100 annotations) */
    nskip = (int) (skp + 1.0);
    iskip = ((int) ((skp + 1.0 - nskip) * 10.0)) % nskip;
    for (z = 0; z < 100; z++) {

/* See if this annotation is being generated */
	if (!linax || z % nskip == iskip) {
	    if (linax || jfmt > 0 || b >= 0.0)
		kfmt = jfmt;
	    else
		kfmt = (int) (0.99 - b);	/* keep at least 1 sign. fig. */
	    switch (yflag) {
	    case 0:		/* x axis */
		x = xpos(val);
		y = annpos;
                in = (x > x1) && (x < x2);
		break;
	    case 1:		/* y axis */
		x = annpos;
		y = ypos(val);
                in = (y > y1) && (y < y2);
		break;
	    case 2:		/* right axis */
		x = annpos;
		y = rpos(val);
                in = (y > y1) && (y < y2);
		break;
	    case 3:		/* upper axis */
		x = tpos(val);
		y = annpos;
                in = (x > x1) && (x < x2);
		break;
	    default:
		return (0.0);
	    }
#ifdef DEBUG
	    if (flags_.dbflg) {
		sprintf(ctext_.msg,
		    " axanno: x,y,val = %9.2f %9.2f %15.6f\n", x, y, val);
		errout(ctext_.msg);
	    }
#endif
            if (in) {
                numlen = numb2(x, y, val, kfmt);
                if (numlen > annoln) {
                    annoln = numlen;
                }
            }
	}
/* Establish the next annotation value and its position */
	if (linax) {		/* Linear axis */
	    if ((z + 1) > b)
		break;
	    val = vmin + c * (z + 1);
	} else {		/* Log axis */
	    if ((b + 0.001) > c)
		break;
	    b = amin1(c, floor(b + 1.001));
	    val = a * pow(logbase, b);
	}
#ifdef DEBUG
	if (flags_.dbflg) {
	    sprintf(ctext_.msg,
		"axanno: a=%f, b=%f, c=%f, val=%f\n", a, b, c, val);
	    errout(ctext_.msg);
	}
#endif
    }
    return (annoln);
}

double
txanno(int yflag, char *vannot, double annpos)
{
    int     j, in;
    char   *s, *e, str[128];
    double  x, y, val, annoln, numlen;
    double  x1, x2, y1, y2;

    e = str + 128 - 1;
    annoln = 0.0;
    x1 = axes_.rxllc - 0.001;
    y1 = axes_.ryllc - 0.001;
    x2 = axes_.rxllc + axes_.rxlen + 0.001;
    y2 = axes_.ryllc + axes_.rylen + 0.001;
    in = 0;
    while (*vannot == ' ')
	vannot++;
    while (*vannot != '\0') {
	for (s = str; s < e && *vannot != ' ' && *vannot != '\0';)
	    *s++ = *vannot++;
	*s = '\0';
	if (getnum(str, &val, ", \t") < 0)
	    val = 0;
	if ((j = indx(str, ',')) > 0) {
	    for (s = str; s[j] != '\0'; s++)
		s[0] = s[j];
	    *s = '\0';
	}
	if (yflag == 0) {
	    x = xpos(val);
	    y = annpos;
            in = (x > x1) && (x < x2);
	} else if (yflag == 1) {
	    x = annpos;
	    y = ypos(val);
            in = (y > y1) && (y < y2);
	} else if (yflag == 2) {
	    x = annpos;
	    y = rpos(val);
            in = (y > y1) && (y < y2);
	} else if (yflag == 3) {
	    x = tpos(val);
	    y = annpos;
            in = (x > x1) && (x < x2);
	} else {
	    return (0.0);
	}
#ifdef DEBUG
	if (flags_.dbflg) {
	    sprintf(ctext_.msg,
		" txanno: x,y,val = %9.2f %9.2f '%s'\n", x, y, str);
	    errout(ctext_.msg);
	}
#endif
        if (in) {
            label(x, y, str);
            numlen = lablen(str);
            if (numlen > annoln) {
                annoln = numlen;
            }
        }
        while (*vannot == ' ') {
	    vannot++;
        }
    }
    return (annoln);
}
