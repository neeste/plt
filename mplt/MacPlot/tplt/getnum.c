/* getnum.c */
/*
 * getnum - get a floating-point number from string
 *
 *	usage: getnum(s, n, del)
 *
 *	where:
 *		s    = points to the string to be parsed
 *		n    = points to a double float which
 *		       will have the interpreted value on output
 *		del  = pointer to array of delimiters
 *
 *	returns number of characters to used from string 's'.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "fort.h"

#define MAXNOPS 8
#define MAXNNBS 8
#define MAXNARG 64

#define IV_DEVICE   0
#define IV_RANDOM   1
#define IV_SUM_XX   2
#define IV_SUM_YY   3
#define IV_SUM_XY   4
#define IV_SUM_X    5
#define IV_SUM_Y    6
#define IV_LR_A     7
#define IV_LR_B     8
#define IV_LR_C     9
#define IV_LR_D     10
#define IV_CORR_XY  11
#define IV_X_MIN    12
#define IV_Y_MIN    13
#define IV_X_MAX    14
#define IV_Y_MAX    15
#define IV_X_MEAN   16
#define IV_Y_MEAN   17
#define IV_X_SD     18
#define IV_Y_SD     19
#define IV_N        20
#define IV_X        21
#define IV_Y        22
#define IV_Z        23
#define NIV         24

#define EQ         128

double  colval(int);
double  gtprmv(int);
double  unirand(void);
double  xdataval(void);
double  ydataval(void);
double  zdataval(void);
double  zdev(double);
double  ztop(double);
double  errfunc(double);
int     evaluate_function(int, int, double *, double *);
int     evaluate_macro(int, int, double *, double *);
int     isalph(char);
int     lookup(char *);
int     lookup_function(char *);
int     lookup_macro(char *);
int     mac_lookup(char *);
int     parse_args(char *, double *, int *);
int     gtdvnm(void);
int     inset(int, char *);
int     ivnmatch(char *);
char   *skipwhite(char *);
char   *skipalphnum(char *);
void    errout(char *);
void    mac_getdef(int, char *);
void    swap_datcol(int, double *);

static int ctof(char *, double *);
static int do_op(double *, char *, int *, int *, int *);
static void expr_err(char *, char *, int);
static int opprec(int);

static float intvar[NIV] = {0};
static char *ivnam[]=
{				/* internal variable names */
    "device", "random",
    "sum_xx", "sum_yy", "sum_xy",
    "sum_x", "sum_y", 
    "lr_a", "lr_b", "lr_c", "lr_d", "corr_xy",
    "x_min", "y_min", "x_max", "y_max",
    "x_mean", "y_mean", "x_sd", "y_sd",
    "n", "x", "y", "z"		/* keep short names last */
};
static int nintvar = sizeof(ivnam) / sizeof(char *);	/* number of key words */

int
getnum(char *ps, double *pn, char *dlm)
{
    char   *s, opst[MAXNOPS];
    double  nbst[MAXNNBS], fa[MAXNARG];
    int     b, i, na, nnbs = 0, nops = 0;

    for (s = ps; !(*s == 0 || inset(*s, dlm));) {
        s = skipwhite(s);
        if (*s == 0 || inset(*s, dlm)) {
            break;
        } else if (inset(*s, "+-*/%^<>&|=~")) {
            if (nops >= MAXNOPS || nops > nnbs) {
                expr_err(ps, s, 1);
                return (-1);
            }
            if (nops < nnbs) {		/* binary op */
                if (inset(*s, "<>=~") && (s[1] == '=')) {
                    opst[nops++] = *s++ + EQ;
                } else if (inset(*s, "=~") || (s[1] == '=')) {
                    expr_err(ps, s, 11);
                    return (-1);
                } else {
                    opst[nops++] = *s;
                }
            } else if (*s == '-') {	/* unary minus */
                nbst[nnbs++] = -1;
                opst[nops++] = '*';
            } else if (*s == '~') {	/* unary negate */
                nbst[nnbs++] = 0;
                opst[nops++] = '~';
            } else if (*s != '+') {	/* unary plus */
                expr_err(ps, s, 1);
                return (-1);
            }
            for (i = 1; i < nops; i++) {
                if (opprec(opst[i - 1]) >= opprec(opst[i])) {
                    if (do_op(nbst, opst, &nnbs, &nops, &i) < 0) {
                        expr_err(ps, s, 2);
                        return (-1);                			
                    }
                }
            }
            s++;
        } else if (*s == '(') {
            if (nnbs >= MAXNNBS || nnbs > nops) {
                expr_err(ps, s, 3);
                return (-1);
            }
            b = getnum(++s, &nbst[nnbs++], ")");
            if (b <= 0)
                return (-1);
            s = skipwhite(s + b);
            if (*s != ')') {
                expr_err(ps, s, 4);
                return (-1);
            }
            s++;
        } else if (*s == ')') {
            expr_err(ps, s, 4);
            return (-1);
        } else if (*s == '[') {
            if (nnbs >= MAXNNBS || nnbs > nops) {
                expr_err(ps, s, 3);
                return (-1);
            }
            b = getnum(++s, &nbst[nnbs++], "]");
            if (b <= 0)
                return (-1);
            s = skipwhite(s + b);
            if (*s != ']') {
                expr_err(ps, s, 4);
                return (-1);
            }
            s++;
        } else if (*s == ']') {
            expr_err(ps, s, 4);
            return (-1);
        } else if (*s == '{') {
            if (nnbs >= MAXNNBS || nnbs > nops) {
                expr_err(ps, s, 3);
                return (-1);
            }
            b = getnum(++s, &nbst[nnbs++], "}");
            if (b <= 0)
                return (-1);
            s = skipwhite(s + b);
            if (*s != '}') {
                expr_err(ps, s, 4);
                return (-1);
            }
            s++;
        } else if (*s == '}') {
            expr_err(ps, s, 4);
            return (-1);
        } else if (strncmp(s, "NaN", 3) == 0) {
            nbst[nnbs++] = nan("");
            s += 3;
        } else if (*s == '$') {
            s++;
            if (*s >= '0' && *s <= '9') {
                i = *s++ - '0';
                if (*s >= '0' && *s <= '9') {
                    i = i * 10 + *s++ - '0';
                }
                nbst[nnbs++] = colval(i);
            } else if ((b = ivnmatch(s)) >= 0) {
                nbst[nnbs++] = intvar[b];
                s += strlen(ivnam[b]);
            } else {
                expr_err(ps, s, 5);
                return (-1);
            }
        } else if (*s == '.' || (*s >= '0' && *s <= '9')) {
            if (nnbs >= MAXNNBS || nnbs > nops) {
                expr_err(ps, s, 3);
                return (-1);
            }
            s += ctof(s, &nbst[nnbs++]);
            while(isalph(*s))
                s++;
        } else if (isalph(*s)) {
            if (nnbs >= MAXNNBS || nnbs > nops) {
                expr_err(ps, s, 3);
                return (-1);
            }
            if ((i = lookup_function(s)) >= 0 && *skipalphnum(s) == '(') {
                if ((b = parse_args(s, fa, &na)) < 0)
                    return (-1);
                s += b;
                i = evaluate_function(i, na, fa, &nbst[nnbs++]);
                if (i != 0)
                    expr_err(ps, s, i);
            } else if ((i = lookup_macro(s)) >= 0 && *skipalphnum(s) == '(') {
                if ((b = parse_args(s, fa, &na)) < 0)
                    return (-1);
                s += b;
                i = evaluate_macro(i, na, fa, &nbst[nnbs++]);
                if (i != 0)
                    expr_err(ps, s, i);
            } else if ((i = lookup(s)) >= 0) {
                nbst[nnbs++] = gtprmv(i);
                s = skipalphnum(s);
            } else {
                expr_err(ps, s, 6);
                return (-1);
            }
        } else {
            expr_err(ps, s, 7);
            return (-1);
        }
        if (nnbs > (nops + 1) || nnbs < (nops - 1))
            return (-1);
    }
    for (i = nops; i > 0;)
        if (do_op(nbst, opst, &nnbs, &nops, &i) < 0)
            return (-1);
    if (nops > 0 || s <= ps)
        return (-1);
    if (nnbs == 1)
        *pn = nbst[0];
    return (int)(s - ps);
}

static int
opprec(int c)
{
    c &= 255;
    if (c == '~')
	return (8);
    if (c == '^')
	return (7);
    if (c == '*' || c == '/' || c == '%')
	return (6);
    if (c == '+' || c == '-')
	return (5);
    if (c == '<' || c == ('<' + EQ) || c == '>' ||  c == ('>' + EQ))
	return (4);
    if (c == ('=' + EQ) ||  c == ('~' + EQ))
	return (3);
    if (c == '&')
	return (2);
    if (c == '|')
	return (1);
    return (0);
}

static int
do_op(double *nbst, char *opst, int *pnnbs, int *pnops, int *pi)
{
    int     i;

    i = (*pi) - 1;
    switch (opst[i] & 255) {
    case '+':
	nbst[i] += nbst[i + 1];
	break;
    case '-':
	nbst[i] -= nbst[i + 1];
	break;
    case '*':
	nbst[i] *= nbst[i + 1];
	break;
    case '/':
	if (nbst[i + 1] == 0.0)
	    return (-1);
	nbst[i] /= nbst[i + 1];
	break;
    case '%':
	nbst[i] = fmod(nbst[i], nbst[i + 1]);
	break;
    case '^':
	nbst[i] = pow(fabs(nbst[i]), nbst[i + 1]);
	break;
    case '<':
	nbst[i] = (nbst[i] < nbst[i + 1]);
	break;
    case '>':
	nbst[i] = (nbst[i] > nbst[i + 1]);
	break;
    case '&':
 	nbst[i] = (nbst[i] && nbst[i + 1]);
	break;
    case '|':
	nbst[i] = (nbst[i] || nbst[i + 1]);
	break;
    case '~':
	nbst[i] = !nbst[i + 1];
	break;
    case ('<' + EQ):
	nbst[i] = (nbst[i] <= nbst[i + 1]);
	break;
    case ('>' + EQ):
	nbst[i] = (nbst[i] >= nbst[i + 1]);
	break;
    case ('=' + EQ):
	nbst[i] = (nbst[i] == nbst[i + 1]);
	break;
    case ('~' + EQ):
	nbst[i] = (nbst[i] != nbst[i + 1]);
	break;
    }
    (*pnnbs)--;
    for (i = *pi; i < *pnnbs; i++)
	nbst[i] = nbst[i + 1];
    (*pnops)--;
    (*pi)--;
    for (i = *pi; i < *pnops; i++)
	opst[i] = opst[i + 1];
    if (*pnnbs < 0 || *pnops < 0 || *pi < 0)
	return (-1);
    return (0);
}

/*
 * unirand - uniform random number generator
 *
 * From "Random Number Generators: Good Ones are Hard to Find",
 * by Stephen K. Park and Keith W. Miller,
 * Communications of the ACM, 31, 10 (Oct. 1988) pp. 1192-1201.
 *
 */
double
unirand(void)
{
    long    hi, lo;
    static long seed = 1;
    static long a = 16807;
    static long m = 2147483647;	/* Mersenne prime 2^31 -1 */
    static long q = 127773;	/* M div A (M / A) */
    static long r = 2836;	/* M mod A (M % A) */

    hi = seed / q;
    lo = seed % q;
    if ((seed = a * lo - r * hi) <= 0)
	seed += m;
    return ((double) seed / m);
}

static int
ctof(char *s, double *v)
{
    char   *b, n;
    double  d, p;

    b = s;
    d = 0;
    while ('0' <= *s && *s <= '9') {
	d *= 10;
	d += *s - '0';
	s++;
    }
    if (*s == '.') {
	s++;
	p = 1;
	while ('0' <= *s && *s <= '9') {
	    p /= 10;
	    d += (*s - '0') * p;
	    s++;
	}
    }
    if (*s == 'e' || *s == 'E') {
	s++;
	n = 0;
	if (*s == '+') {
	    s++;
	} else if (*s == '-') {
	    n = 1;
	    s++;
	}
	p = 0;
	while ('0' <= *s && *s <= '9') {
	    p *= 10;
	    p += *s - '0';
	    s++;
	}
	if (p > 0)
	    d *= pow(10.0, n ? -p : p);
    }
    *v = d;
    return (int)(s - b);
}

#define FN_SIN  0
#define FN_COS  1
#define FN_TAN  2
#define FN_LOG  3
#define FN_LN   4
#define FN_MIN  5
#define FN_MAX  6
#define FN_ABS  7
#define FN_LMT  8
#define FN_ZDV  9
#define FN_XDA  10
#define FN_YDA  11
#define FN_ZDA  12
#define FN_IFE  13
#define FN_Z2P  14
#define FN_ATN  15
#define FN_SQR  16
#define FN_FLR  17
#define FN_CEL  18
#define FN_ERF  19
#define FN_EXP  20
#define FN_CMS  21
#define FN_CMR  22
#define FN_CMI  23
#define FN_CDR  24
#define FN_CDI  25
#define FN_TNH  26
#define FN_ATH  27
#define FN_SEL  28
#define FN_ROU  29
#define FN_XSV  30
#define FN_YSV  31
#define FN_ZSV  32

int
toklen(char *s)
{
    int n = 0;
    
    while(isalnum(s[n]))
        n++;
    return(n);
}

int
lookup_function(char *s)
{
    int     i;
    static char *fn[] = {
	"sin",
	"cos",
	"tan",
	"log",
	"ln",
	"min",
	"max",
	"abs",
	"limit",
	"zdev",
	"xdat",
	"ydat",
	"zdat",
	"ifelse",
	"ztop",
	"atan",
	"sqrt",
	"floor",
	"ceil",
	"erf",
	"exp",
	"cms",
	"cmr",
	"cmi",
	"cdr",
	"cdi",
	"tanh",
	"atanh",
	"select",
	"round",
	"xsav",
	"ysav",
	"zsav",
    };
    static int nfn = sizeof(fn) / sizeof(fn[0]);

    for (i = 0; i < nfn; i++)
	if (strncmp(s, fn[i], strlen(fn[i])) == 0)
	    if (strlen(fn[i]) == (unsigned) toklen(s))
	        return (i);
    return (-1);
}

int
evaluate_function(int i, int n, double *a, double *v)
{
    extern int ndatp;
    extern float *xdat, *ydat, *zdat;
    extern float *xsav, *ysav, *zsav;

    if (n <= 0)
	return (8);
    switch (i) {
    case FN_SIN:
	*v = sin(*a);
	break;
    case FN_COS:
	*v = cos(*a);
	break;
    case FN_TAN:
	*v = tan(*a);
	break;
    case FN_LOG:
	if (*a <= 0)
	    return (9);
	*v = log10(*a);
	break;
    case FN_LN:
	if (*a <= 0)
	    return (9);
	*v = log(*a);
	break;
    case FN_MIN:
	*v = a[--n];
	while (--n >= 0)
	    if (*v > a[n])
		*v = a[n];
	break;
    case FN_MAX:
	*v = a[--n];
	while (--n >= 0)
	    if (*v < a[n])
		*v = a[n];
	break;
    case FN_ABS:
	*v = (a[0] < 0) ? -a[0] : a[0];
	break;
    case FN_LMT:
	*v = (a[1] < a[0]) ? a[0] : (a[1] > a[2]) ? a[2] : a[1];
	break;
    case FN_ZDV:
	*v = zdev(a[0]);
	break;
    case FN_XDA:
        *v = (a[0] < 1 || a[0] > ndatp) ? 0 : xdat[(int) a[0] - 1];
	break;
    case FN_YDA:
        *v = (a[0] < 1 || a[0] > ndatp) ? 0 : ydat[(int) a[0] - 1];
	break;
    case FN_ZDA:
        *v = (a[0] < 1 || a[0] > ndatp) ? 0 : zdat[(int) a[0] - 1];
	break;
    case FN_IFE:
	*v = a[0] ? a[1] : a[2];
	break;
    case FN_Z2P:
	*v = ztop(a[0]);
	break;
    case FN_ATN:
	*v = (n < 2) ? atan(a[0]) : atan2(a[0], a[1]);
	break;
    case FN_SQR:
	*v = (a[0] > 0) ? sqrt(a[0]) : 0;
	break;
    case FN_FLR:
	*v = floor(a[0]);
	break;
    case FN_CEL:
	*v = ceil(a[0]);
	break;
    case FN_ERF:
	*v = errfunc(a[0]);
	break;
    case FN_EXP:
	*v = exp(a[0]);
	break;
    case FN_CMS:
	*v = a[0] * a[0] + a[1] * a[1];
	break;
    case FN_CMR:
	*v = a[0] * a[2] - a[1] * a[3];
	break;
    case FN_CMI:
	*v = a[1] * a[2] + a[0] * a[3];
	break;
    case FN_CDR:
	*v = (a[0] * a[2] + a[1] * a[3]) / (a[2] * a[2] + a[3] * a[3]);
	break;
    case FN_CDI:
	*v = (a[1] * a[2] - a[0] * a[3]) / (a[2] * a[2] + a[3] * a[3]);
	break;
    case FN_TNH:
	*v = tanh(a[0]);
	break;
    case FN_ATH:
	*v = atanh(a[0]);
	break;
    case FN_SEL:
	if (a[0] >= 0 && a[0] <= n)
	    *v = a[nint(a[0])];
	else
	    *v = 0;
	break;
    case FN_ROU:
	*v = floor(a[0] + 0.5);
	break;
    case FN_XSV:
        *v = (!xsav || a[0] < 1 || a[0] > ndatp) ? 0 : xsav[(int) a[0] - 1];
	break;
    case FN_YSV:
        *v = (!ysav || a[0] < 1 || a[0] > ndatp) ? 0 : ysav[(int) a[0] - 1];
	break;
    case FN_ZSV:
        *v = (!zsav || a[0] < 1 || a[0] > ndatp) ? 0 : zsav[(int) a[0] - 1];
	break;
    }
    return (0);
}

int
parse_args(char *s, double *v, int *n)
{
    int     b, c;
    char   *t;

    c = 0;
    t = skipalphnum(s);
    if (*t == '(') {
        do {
            if (c >= MAXNARG)
                return (-1);
            b = getnum(++t, v++, ",)");
	    if (b <= 0)
		return (-1);
	    t += b;
	    c++;
	} while (*t == ',');
	t++;
    }
    *n = c;
    return (int)(t - s);
}

int
lookup_macro(char *s)
{
    char   *p;
    int     i = -1;

    if ((p = strchr(s, '(')) != NULL) {
	*p = '\0';
	i = mac_lookup(s);
	*p = '(';
    }
    return (i);
}

int
evaluate_macro(int i, int n, double *a, double *v)
{
    int     b;
    char    s[256];

    swap_datcol(n, a);
    mac_getdef(i, s);
    b = getnum(s, v, "");
    swap_datcol(n, a);
    return ((b > 0) ? 0 : 10);
}

static void
expr_err(char *s, char *e, int c)
{
    int     i, j, m, n;
    char    msg[256];
    static char *ms = "| error in expression: ";
    static int errcnt = 0;

    if (errcnt++ > 100)
	return;
    j = (int)(255 - strlen(ms));
    n = (int) (e - s);
    if (n > j)
	s[j] = '\0';
    sprintf(msg, "%s%s\n", ms, s);
    errout(msg);
    if (n < j) {
	m = (int) (n + strlen(ms));
	msg[0] = '|';
	for (i = 1; i < m; i++)
	    msg[i] = ' ';
	msg[i++] = '^';
	msg[i++] = '\n';
	msg[i] = '\0';
	errout(msg);
    }
    switch (c) {
    case 1:
	e = "operators with no numbers";
	break;
    case 2:
	e = "can't do operation";
	break;
    case 3:
	e = "numbers with no operator";
	break;
    case 4:
	e = "unbalanced parentheses";
	break;
    case 5:
	e = "unknown internal variable";
	break;
    case 6:
	e = "unknown function or variable";
	break;
    case 7:
	e = "unexpected character";
	break;
    case 8:
	e = "function has no arguments";
	break;
    case 9:
	e = "log argument is zero or negative";
	break;
    case 10:
	e = "unable to evaluate macro";
	break;
    case 11:
	e = "invalid binary operator";
	break;
    }
    sprintf(msg, "| -> %s\n", e);
    errout(msg);
}

/**********************************************************************/

int
ivnmatch(char *s)
{
    int     i, n = -1;

    for (i = 0; i < nintvar; i++)
        if (strncmp(s, ivnam[i], strlen(ivnam[i])) == 0) {
            n = i;
            break;
        }
    switch (n) {
    case IV_X:
        intvar[n] = (float) xdataval();
        break;
    case IV_Y:
	intvar[n] = (float) ydataval();
	break;
    case IV_Z:
	intvar[n] = (float) zdataval();
	break;
    case IV_DEVICE:
	intvar[n] = (float) gtdvnm();
	break;
    case IV_RANDOM:
	intvar[n] = (float) unirand();
	break;
    }
    return (n);
}

void
cstat(void)
{
    int     i, n;
    float   sx, sy, sxx, syy, sxy, detx, dety;
    float   xmn, ymn, xmx, ymx;
    extern int ndatp;
    extern float *xdat, *ydat;

    n = ndatp;
    sx = sy = 0;
    sxx = syy = sxy = 0;
    xmn = xmx = xdat[0];
    ymn = ymx = ydat[0];
    for (i = 0; i < n; i++) {
	sx += xdat[i];
	sy += ydat[i];
    }
    for (i = 0; i < n; i++) {
	sxx += xdat[i] * xdat[i];
	syy += ydat[i] * ydat[i];
	sxy += xdat[i] * ydat[i];
    }
    for (i = 0; i < n; i++) {
	if (xmn > xdat[i])
	    xmn = xdat[i];
	if (ymn > ydat[i])
	    ymn = ydat[i];
	if (xmx < xdat[i])
	    xmx = xdat[i];
	if (ymx < ydat[i])
	    ymx = ydat[i];
    }
    detx = n * sxx - sx * sx;
    dety = n * syy - sy * sy;

    intvar[IV_N] = (float) n;
    intvar[IV_SUM_X] = sx;
    intvar[IV_SUM_Y] = sy;
    intvar[IV_SUM_XX] = sxx;
    intvar[IV_SUM_YY] = syy;
    intvar[IV_SUM_XY] = sxy;
    intvar[IV_X_MIN] = xmn;
    intvar[IV_Y_MIN] = ymn;
    intvar[IV_X_MAX] = xmx;
    intvar[IV_Y_MAX] = ymx;
    if (n <= 0) {
	intvar[IV_X_MEAN] = 0;
	intvar[IV_Y_MEAN] = 0;
	intvar[IV_X_SD] = 0;
	intvar[IV_Y_SD] = 0;
    } else {
	intvar[IV_X_MEAN] = sx / n;
	intvar[IV_Y_MEAN] = sy / n;
	intvar[IV_X_SD] = (float) sqrt(detx) / n;
	intvar[IV_Y_SD] = (float) sqrt(dety) / n;
    }
    if (detx == 0 || dety == 0) {
	intvar[IV_LR_A] = 0;
	intvar[IV_LR_B] = 0;
	intvar[IV_LR_C] = 0;
	intvar[IV_LR_D] = 0;
	intvar[IV_CORR_XY] = 0;
    } else {
	intvar[IV_LR_A] = (sy * sxx - sx * sxy) / detx;
	intvar[IV_LR_B] = (n * sxy - sx * sy) / detx;
	intvar[IV_LR_C] = (sx * syy - sy * sxy) / dety;
	intvar[IV_LR_D] = (n * sxy - sx * sy) / dety;
	intvar[IV_CORR_XY] = (n * sxy - sx * sy) / (float) sqrt(detx * dety);
    }
}

/************************************************************************/
/*                                                                      */
/*     zdev - C version of zdev.for from wjlib                          */
/*                                                                      */
/*   Translator:         J. Peters                                      */
/*                                                                      */
/*      Date of first version:  Mar. 10, 1989                           */
/*      Date of this  version:  Mar. 12, 1993                           */
/*                                                                      */
/*   Purpose: To convert from a percentile to a z score for a unit      */
/*         normal distribution.                                         */
/*                                                                      */
/*      Usage:   z = zdev(p)                                            */
/*         p:   is proportion or percentile passed to routine.          */
/*              p is assumed to be n the range from 0 to 1              */
/*         z:   the z score                                             */
/*                                                                      */
/*      Functions required: log, exp in <math.h> lib                    */
/*                                                                      */
/*      Notes to programmers: Uses Hastings approximation.              */
/*                                                                      */
/*         Note that if P is 0.0 or 1.0, it is set to .0001 or          */
/*         to .9999.  Ideally, this fudge will be avoided in the        */
/*         calling routine by applying some correction that is          */
/*         proportional to the n on which the estimate is based.        */
/*         The correction is done here only as a last resort.           */
/*                                                                      */
/*      Other documentation:                                            */
/*         Hastings, Approximations for Digital Computers.              */
/*                                                                      */
/************************************************************************/

double
zdev(double p)
{
    double  p1, sz, xa, xb, xc, z;

    if (p == 0.5) {
	z = 0;
    } else {
	if (p > 0.5) {
	    p1 = p - 0.5;
	    sz = 1;
	} else {
	    p1 = 0.5 - p;
	    sz = -1;
	}
	if (p1 >= 0.5)
	    p1 = 0.4999;
	xb = sqrt(-2.0 * log((double) .5 - p1));
	xc = 2.515517 + xb * (0.802853 + 0.010328 * xb);
	xa = 1.0 + xb * (1.432788 + xb * (0.189269 + .001308 * xb));
	z = sz * (xb - xc / xa);
    }
    return (z);
}

double 
ztop(double z)
{
    double v, za, zb, p;
    static double sqrt2 = 1.41421356237;
    static double c1 = 0.0705230784;
    static double c2 = 0.04228201;
    static double c3 = 0.0092705272;
    static double c4 = 0.0001520143;
    static double c5 = 0.0002765672;
    static double c6 = 0.0000430638;
 
    v = ((z < 0) ? -z : z) / sqrt2;
    za = c5 + v * c6;
    za = c4 + v * za;
    za = c3 + v * za;
    za = c2 + v * za;
    za = c1 + v * za;
    zb = 1 / (1 + v * za);
    zb *= zb;       /* ^2  */
    zb *= zb;       /* ^4  */
    zb *= zb;       /* ^8  */
    zb *= zb;       /* ^16 */
    zb /= 2;
    p = (z < 0) ? zb : 1 - zb;
    return (p);
}

/* error function - from Abramowitz and Stegun (1964) Eq. 7.1.26 */
double 
errfunc(double x)
{
    double t, c, erf;
    static double p = 0.3275911;
    static double a1 = 0.254829592;
    static double a2 = -0.284496736;
    static double a3 = 1.421413741;
    static double a4 = -1.453152027;
    static double a5 = 1.061405429;

    if (x == 0) {
	erf = 0;
    } else if (x < 0) {
	erf = -errfunc(-x);
    } else {
	t = 1 / (1 + p * x);
	c = a4 + t * a5;
	c = a3 + t * c;
	c = a2 + t * c;
	c = a1 + t * c;
	erf = 1 - t * c * exp(-x * x);
    }
    return (erf);
}
