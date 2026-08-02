/*
 * opninc - open include file
 */
#include <string.h>
#include <stdlib.h>
#include "plt.h"
#include "pltblk.h"
#include "cincl.h"

int     indx(char *, int);
int     ldfont(char *, int);
int     noext(char *);
int     newext(char *, char *);
void    namfil(char *, char *);
void    getfn(char *, char *, int, char *);
void    getpn(char *, char *);

void
opninc(char *instr)
{
    char    fn[NAMESIZE];
    char    pn[NAMESIZE];

    if (cincl_.inclev >= MAXINCL) {	/* check nesting depth */
	sprintf(ctext_.msg, " Illegal CALL/INCLUDE nesting\n");
	errout(ctext_.msg);
	return;
    }
    getfn(instr, fn, NAMESIZE, ".plt");
    getpn(pn, fn);

    cincl_.inc_file[cincl_.inclev] = misc_.lunin;
    if ((misc_.lunin = fopen(pn, "r")) == NULL) {
	sprintf(ctext_.msg,
	    " *** can't open INCLUDE or CALL file: %s\n", pn);
	errout(ctext_.msg);
	misc_.lunin = cincl_.inc_file[cincl_.inclev];
	return;
    }
    cincl_.inclev++;
    misc_.lftovr = 0;

#ifdef DEBUG
    if (flags_.dbflg) {
	sprintf(ctext_.msg,
	    " opninc: include file '%s', inclev == %d, lunin==%X\n",
	    fn, cincl_.inclev, (unsigned int)misc_.lunin);
	errout(ctext_.msg);
    }
#endif
}

void
opnfnt(char *instr, int fnttyp)
{
    int     i, status = 0;
    char    fn[NAMESIZE], pn[NAMESIZE], *pfd;
    static char *dex = ".pft";
#ifdef _MSC_VER
    static char *dirlst[]= {
	"", "c:/lib/", "c:/fonts/"
    };
#else
  static char *dirlst[]= {
    "", "/usr/local/lib/pltfont/", "/usr/src/local/plt/pltfnt/"
  };
#endif
    static int ndl = sizeof(dirlst) / sizeof(dirlst[0]);

    getfn(instr, fn, NAMESIZE, dex);
    if (strcmp(fn, dex) == 0) {		/* if no file name */
	(void) ldfont("", fnttyp);	/* use internal font */
	return;
    }
#ifdef DEBUG
    if (flags_.dbflg) {
	sprintf(ctext_.msg,
	    " opnfnt: font file '%s', fnttyp == %d\n", fn, fnttyp);
	errout(ctext_.msg);
    }
#endif

    if (strlen(fn) == 0) {
	status = ldfont(fn, fnttyp);
    } else if (indx(fn, '/') || indx(fn, '\\') || indx(fn, ':')) {
	strcpy(pn, fn);
	status = ldfont(pn, fnttyp);
    } else if ((pfd = getenv("PLT_FONTS")) != NULL) {
	strcpy(pn, pfd);
	strcat(pn, "/");
	strcat(pn, fn);
	status = ldfont(pn, fnttyp);
    } else {
	getpn(pn, fn);
	status = ldfont(pn, fnttyp);
	if (status == 1) {
	    for (i = 0; i < ndl; i++) {
		strcpy(pn, dirlst[i]);
		strcat(pn, fn);
		status = ldfont(pn, fnttyp);
		if (status != 1)
		    break;
	    }
	}
    }

    switch (status) {
    case 0:
	return;
    case 1:
	sprintf(ctext_.msg, " *** can't open FONT file: %s\n", fn);
	break;
    case 2:
	sprintf(ctext_.msg,
	    " *** not enough memory to load FONT file: %s\n", fn);
	break;
    case 3:
	sprintf(ctext_.msg,
	    " *** FONT file has incorrect format: %s\n", pn);
	break;
    }
    errout(ctext_.msg);
}

void
getpn(char *pn, char *fn)
{ 
    /* check if file name contains absolute path or device name */

    if (*fn != '/' && *fn != '\\' && *fn != '.' && !indx(fn, ':')
	&& *misc_.curpltdir != '\0') {
	strcpy(pn, misc_.curpltdir);
	strcat(pn, fn);
    } else {
	strcpy(pn, fn);
    }
}

void
getfn(char *instr, char *fn, int nc, char *de)
{
    char   *pf, *ps, *pe;

    while (*instr == ' ' || *instr == '\t') /* skip whitespace */
	instr++;
    for (ps = instr; *ps != ' ' && *ps != '\t' && *ps != '\0'; ps++) /* skip keyword */
        continue;
    while (*ps == ' ' || *ps == '\t')	    /* skip whitespace */
        ps++;
    if (*ps == '*') {		/* If file name input requested */
	namfil(fn, de);
    } else {			/* parse filename */
	pe = fn + nc - 1;
	for (pf = fn; (*ps >= ' ' && pf < pe);)	/* allow spaces */
	    *pf++ = *ps++;
	while ((pf > fn) && (pf[-1] < ' '))	/* trim spaces from end */
	    pf--;
	*pf = '\0';
    }
    if (*fn && noext(fn)) {	/* if no filename extension */
        newext(fn, de);		/* add the default extension */
    }
}
