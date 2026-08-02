/* lookup.c - PLT function to scan LIST of praramter names
 *
 *	USAGE:		i = lookup(s)
 *
 *	Where:
 *		      s  = pointer to parameter name
 */
#include <string.h>
#include "plt.h"
#include "pltblk.h"

#define NVAR    100
#define	VAROFF	1000
#define	VNLN	4
#define	VNSZ	(VNLN+1)

int    chrupr(int);
int    isalph(char);
int    isalphnum(char);
int    iscan(char *[], int, char *);

static char *prmnam[] = {
    "ANNC", "ANNL", "ANNS", "ARON", "AROS", "AROT", "AROW", "AXCO", 
    "AXLW", "BGCO", "CDEX", "CLIP", "CORN", "COUN", "EBNU", "EBWI", 
    "ECHO", "FGCO", "FILC", "FNCO", "GAPS", "GRDC", "GRDL", "GRDS", 
    "GRDT", "GRID", "HEAD", "LABC", "LABL", "LABS", "LIKE", "LIND", 
    "LINS", "LINT", "MAGN", "MHAL", "MHKE", "MHSP", "MHXP", "MSGA", 
    "MSGC", "MSGD", "MSGL", "MSGS", "MSLA", "MVAL", "MVKE", "MVSP", 
    "MXDA", "MXLL", "MYDA", "MYLL", "NDAT", "NORM", "OPEN", "PLTC", 
    "PLTL", "PLTY", "RANS", "RAXI", "RBRE", "RCYC", "RINT", "RMAX", 
    "RMIN", "ROTA", "RPER", "RSHI", "RTIC", "SHAD", "SHDC", "SHDG", 
    "SHDL", "SIZF", "SMOO", "SOLI", "SORT", "SPLI", "STEM", "SYMA", 
    "SYMB", "SYMS", "TANS", "TAXI", "TBRE", "TCYC", "TICD", "TICS", 
    "TINT", "TMAX", "TMIN", "TPER", "TSHI", "TTIC", "VXLL", "VYLL", 
    "WXLE", "WXLL", "WYLE", "WYLL", "XANS", "XAXI", "XBRE", "XCYC", 
    "XGAI", "XINT", "XLEN", "XLLC", "XMAX", "XMIN", "XOFS", "XPER", 
    "XSHI", "XTIC", "YANS", "YAXI", "YBRE", "YCYC", "YGAI", "YHOR", 
    "YINT", "YLEN", "YLLC", "YMAX", "YMIN", "YOFS", "YPER", "YSHI", 
    "YTIC", "ZGAI", "ZOFS"
}; 
static char *varnam[NVAR] = {NULL};
static float *prmval = (float *) &param_; 
static float varval[NVAR] = {0};
static int npnam = sizeof(prmnam) / sizeof(prmnam[0]);	/* number of pars */
static int nvnam = 0;					/* number of vars */

int
lookup(char *s)
{
    int n;

    n = iscan(prmnam, npnam, s) - 1;
    if (n < 0) {
	n = iscan(varnam, nvnam, s) - 1;
	if (n >= 0)
	    n += VAROFF;
    }
#ifdef DEBUG
    if (flags_.dbflg) {
        sprintf(ctext_.msg, "lookup: s='%s' n=%d\n", s, n);
        errout(ctext_.msg);
    }
#endif /* DEBUG */
    return (n);
}

int
insnam(char *s)
{
    int i;

    if (isalph(*s)) {
        if (nvnam < NVAR) {
            varnam[nvnam] = (char*)calloc(VNSZ, 1);
            for (i = 0; i < VNLN && isalphnum(s[i]); i++)
                varnam[nvnam][i] = chrupr(s[i]);
            if (isalphnum(s[i])) {
                sprintf(ctext_.msg, "new variable name is too long: %s\n", s);
                errout(ctext_.msg);
            }
            varnam[nvnam][i] = '\0';
            nvnam++;
#ifdef DEBUG
            if (flags_.dbflg) {
                sprintf(ctext_.msg, "insnam: s='%s' nv=%d\n", s, nvnam);
                errout(ctext_.msg);
            }
#endif /* DEBUG */
	}
	return (VAROFF + nvnam - 1);
    }
    return (0);
}

char *
luname(int i)
{
    if (i >= 0 && i < npnam) {
	return (prmnam[i]);
    } else if ((i >= VAROFF) && (i < (VAROFF + nvnam))) {
	return (varnam[i]);
    }
    return (NULL);
}

int
numpar(void)
{
    return (npnam);
}

void
getpar(int i, char *pn, float *pv)
{
    if (i >= 0 && i < npnam) {
	strncpy(pn, prmnam[i], VNSZ);
	*pv = prmval[i];
    }
}

int
numvar(void)
{
    return (nvnam);
}

void
clrvar(void)
{
    while(nvnam > 0) {
	nvnam--;
	free(varnam[nvnam]);
	varnam[nvnam] = NULL;
    }
}

/* ordval - sort variable names in alphabetical order */
void
ordvar(void)
{
    char    tmpnam[VNSZ];
    float   tmpval;
    int     i, j;

    for (i = 0; i < nvnam; i++) {
	for (j = i + 1; j < nvnam; j++) {
	    if (strcmp(varnam[j], varnam[i]) < 0) { /* exchange order ? */
		strcpy(tmpnam, varnam[i]);
		strcpy(varnam[i], varnam[j]);
		strcpy(varnam[j], tmpnam);
		tmpval = varval[i];
		varval[i] = varval[j];
		varval[j] = tmpval;
	    }
	}
    }
}

void
getvar(int i, char *vn, float *vv)
{
    if (i >= 0 && i < nvnam) {
	strncpy(vn, varnam[i], VNSZ);
	*vv = varval[i];
    }
}

double
gtprmv(int i)
{
    double  v = 0.0;

    if (i >= 0 && i < npnam) {
	v = prmval[i];
    } else {
	i -= VAROFF;
	if (i >= 0 && i < nvnam)
	    v = varval[i];
    }
    return (v);
}

void
stprmv(int i, double v)
{
    if (i >= 0 && i < npnam) {
	prmval[i] = (float) v;
    } else {
	i -= VAROFF;
	if (i >= 0 && i < nvnam)
	    varval[i] = (float) v;
    }
}
