/* wrprml.c - write parameter list to errout */

#include <string.h>
#include "plt.h"
#include "pltblk.h"

int     numpar(void);
int     numvar(void);
void    getfn(char *, char *, int, char *);
void    getpar(int, char *, float *);
void    getvar(int, char *, float *);
void    ordvar(void);

void
wrprml(void)
{
    float   pv;
    int     i, n;
    char    pn[5], s[64];

    n = numpar();
    sprintf(ctext_.msg, "\nCurrent Parameter Values [%d]:\n", n);
    for (i = 0; i < n; i++) {
    	getpar(i, pn, &pv);
	if ((i % 5) > 0) {
	    strcat(ctext_.msg, "|");
	} else {
	    strcat(ctext_.msg, "\n");
	    errout(ctext_.msg);
	    ctext_.msg[0] = '\0';
	}
	sprintf(s, " %4.4s %8.3g ", pn, pv);
	strcat(ctext_.msg, s);
    }
    if (i > 0) {
	strcat(ctext_.msg, "\n");
	errout(ctext_.msg);
    }
}

void
wrdatl(int npt, float *x, float *y, float *z, char *s)
{
    int     i;
    char    fn[NAMESIZE];
    FILE    *fp;

    if (npt <= 0) {
	errout(" Data Buffer Empty\n");
    } else {
        getfn(s, fn, NAMESIZE, "");
        if (*fn == 0 || (fp = fopen(fn, "w")) == NULL) {
            sprintf(ctext_.msg, "\n Current Data Values [%d]: (I,X,Y,Z)\n",
                npt);
            errout(ctext_.msg);
	    for (i = 0; i < npt; i++) {
	        sprintf(ctext_.msg, "%4d %10.3f %10.3f %10.3f\n",
		    i, x[i], y[i], z[i]);
	        errout(ctext_.msg);
	    }
        } else {
            fprintf(fp, "; Current Data Values [%d]: (I,X,Y,Z)\n", npt);
	    for (i = 0; i < npt; i++) {
	        fprintf(fp, "%4d %10.3f %10.3f %10.3f\n",
		    i, x[i], y[i], z[i]);
	    }
            fclose(fp);
        }
    }
}

void
wrvarl(void)
{
    float   vv;
    int     i, n;
    char    vn[5], s[64];

    ordvar();
    n = numvar();
    sprintf(ctext_.msg, "\nCurrent Variable Values [%d]:\n", n);
    for (i = 0; i < n; i++) {
	getvar(i, vn, &vv);
	if ((i % 5) > 0) {
	    strcat(ctext_.msg, "|");
	} else {
	    strcat(ctext_.msg, "\n");
	    errout(ctext_.msg);
	    ctext_.msg[0] = '\0';
	}
	sprintf(s, " %4.4s %8.3g ", vn, vv);
	strcat(ctext_.msg, s);
    }
    if (i > 0) {
	strcat(ctext_.msg, "\n");
	errout(ctext_.msg);
    }
}
