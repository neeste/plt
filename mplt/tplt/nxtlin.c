/* nxtlin.c */
/*
 *
 * 	function nxtlin (INSTR)
 *
 *
 *	DATE OF  FIRST  VERSION:	 4-Apr-80
 *	DATE OF CURRENT VERSION:	21-Jun-83
 *
 *	PURPOSE:	PLTFIL routine to read the next line from
 *			the inut file and determine its type.
 *
 *	USAGE:	ICMD = NXTLIN (INSTR)
 *
 *		Where	INSTR -- Input line returned here (byte array)
 *
 *	PROGRAMMER:	M. Collins / E. Javel / M. Prucka
 **********************************************************************
 */
#include <string.h>
#include <stdlib.h>
#include "plt.h"
#include "pltblk.h"

#define COMMENT 0
#define DATUM 1
#define KEY 2
#define PARAM 3
#define MESSAGE 4
#define MACRO 5
#define UNKNOWN 99

#define MXNARG  64
#define MXNMAC  64

int     indx(char *, int);
int     getnum(char *, double *, char *);
int     get_str(FILE *, char *, int);
int     mac_lookup(char *);
void    do_macro(char *);
void    mac_parse(char *, char **, int *);
void    mac_install(int, char **);
void    mac_undefine(int, char **);
void    mac_repeat(int, char **);
void    mac_for(int, char **);
void    mac_msg(int, char **);
void    mac_pubdef(int, int, char **);
void    pubchr(int);
void    pubstr(char *);

static int isalfa(char *);
static int isdata(char *);

int
nxtlin(char *instr)
{
    char   *cmdstr;
    int     ier, cmdtype, j, nc;

/*
 *---------------------------------------------------------------------
 *
 */

/*
 * Read input lines until we succeed in getting a good one
 */
    do {
	if (misc_.lftovr > 0) {
	    cmdstr = instr + misc_.lftovr;
	} else {
	    ier = get_str(misc_.lunin, instr, MAXLINE);
	    if (ier != 0)
		return (EOF);
	    if (param_.echo != 0.0) {	/* Echo the input line, if so desired */
		sprintf(ctext_.msg, "%s\n", instr);
		errout(ctext_.msg);
	    }
	    cmdstr = instr;
	}
/*
 * Remove leading blanks and control characters.
 */
	while (*cmdstr <= ' ' && *cmdstr != '\0')
	    cmdstr++;
/*
 * Look for an internal comment ';' and eliminate it from the line.
 * If the line so created is blank, ignore it and get another one.
 * Allow the ';' to be escaped by a '\'.
 */
	if ((j = indx(cmdstr, ';')) > 0) {
	    if (j == 1 || cmdstr[j - 2] != '\\') {
		cmdstr[j - 1] = '\0';
	    }
	}
/*
 * Look for an external comment '!' and print it if that has not been done.
 * Then, eliminate the external comment portion and check to see if
 * the elimination resulted in a blank line; if so, ignore it.
 * Allow the '!' to be escaped by a '\'.
 */
	if ((j = indx(cmdstr, '!')) > 0) {
	    if (j == 1 || cmdstr[j - 2] != '\\') {
		if (param_.echo == 0.0) {
		    sprintf(ctext_.msg, "%s\n", cmdstr + j - 1);
		    errout(ctext_.msg);
		}
		cmdstr[j - 1] = '\0';
	    }
	}
/*
 * Trim spaces and control characters from the end of cmdstr
 */
	if ((nc = (int)strlen(cmdstr)) > 0) {
	    for (j = nc; cmdstr[j - 1] <= ' '; j--)
		continue;
	    cmdstr[j] = '\0';
	}
/*
 * It is a real line, so decode the statement type by checking for
 * implied statement types:
 */
	if (strlen(cmdstr) == 0) {	/* Blank lines are comments */
	    cmdtype = COMMENT;
	} else if (indx(cmdstr, '%') == 1) {	/* Macros begin with percent */
	    cmdtype = MACRO;
	} else if (indx(cmdstr, '"') > 0) {	/* Message has quote mark */
	    cmdtype = MESSAGE;
	} else if (indx(cmdstr, '=') > 0) {	/* Parameter has equals sign */
	    cmdtype = PARAM;
	} else if (isalfa(cmdstr)) {	/* Keywords are alphabetic */
	    cmdtype = KEY;
	} else if (isdata(cmdstr)) {	/* Check for data value.  */
	    cmdtype = DATUM;
	} else {		/* Unknown type of command. */
	    cmdtype = UNKNOWN;
	}
/*
 * Do macros here without exiting input loop
 */
	if (cmdtype == MACRO)
	    do_macro(cmdstr);
/*
 * End of input loop
 */
    } while (cmdtype == COMMENT || cmdtype == MACRO);
/*
 * Copy from cmdstr to instr to remove leading blanks.
 */
    if (cmdstr > instr) {
        nc = (int)strlen(cmdstr);
        for (j = 0; j <= nc; j++) { /* strcpy unreliable */
            instr[j] = cmdstr[j];
        }
    }
/*
 * Allow multiple parameter statements per line seprated by ' :'.
 * Save remainder of line for subsequent calls to nxtlin routine.
 */
    if (cmdtype == PARAM
	&& ((j = indx(instr, ':')) > (indx(instr, '=') + 1))
	&& (instr[j - 2] == ' ' || instr[j - 2] == '\t')) {
	misc_.lftovr = (instr[j] == '\0') ? 0 : j;
	while (instr[j - 3] == ' ' || instr[j - 3] == '\t')
	    j--;
	instr[j - 2] = '\0';
    } else {
	misc_.lftovr = 0;
    }
#ifdef DEBUG
    if (flags_.dbflg) {
	sprintf(ctext_.msg, "nxtline: cmdtyp=%d, lftovr=%d, instr='%s'\n",
	    cmdtype, misc_.lftovr, instr);
	errout(ctext_.msg);
    }
#endif
    return (cmdtype);
}

static int
isalfa(char *s)
{
    return (('A' <= *s && *s <= 'Z') || ('a' <= *s && *s <= 'z'));
}

static int
isdata(char *s)
{
    return (('0' <= *s && *s <= '9')
	|| *s == '+' || *s == '-' || *s == '.' || *s == '(' || *s == '*');
}

static int
is_seq(s)
char   *s;
{
    int ic, n1, n2, nd;

    ic = indx(s, ':');
    if (ic > 0) {
        n1 = atoi(s);
        n2 = atoi(s + ic);
        nd = abs(n2 - n1);
        if ((n1 > 0) && (n2 > 0) && (nd < MXNARG)) {
            return (1);
        }
    }
    return (0);
}

/*
 * macro routines
 */

static struct {
    char   *nam, *def;
}       md[MXNMAC];
static int nmd = 0;
static char *nulstr = "";

void
do_macro(char *cmdstr)
{
    int     ac, mn, mc;
    char   *cs, *av[MXNARG];

    if ((mc = indx(cmdstr, '%')) == 0)
	return;
    cs = cmdstr + mc;
    mac_parse(cs, av, &ac);
    if (strcmp(av[0], "define") == 0) {
	mac_install(ac, av);
    } else if (strcmp(av[0], "undefine") == 0) {
	mac_undefine(ac, av);
    } else if (strcmp(av[0], "repeat") == 0) {
	mac_repeat(ac, av);
    } else if (strcmp(av[0], "for") == 0) {
	mac_for(ac, av);
    } else if (strcmp(av[0], "msg") == 0) {
	mac_msg(ac, av);
    } else if ((mn = mac_lookup(av[0])) >= 0) {
	mac_pubdef(mn, ac, av);
    }
}

void
mac_parse(char *cs, char **av, int *pac)
{
    char   *s;
    int     n, q;

    n = 0;
    for (s = cs; *s != '\0' && n < MXNARG;) {
	while (*s <= ' ' && *s != '\0')
	    s++;
	if ((q = (*s == '"')))
	    s++;
	av[n++] = s;
	if (q) {
	    while (*s != '"' && *s != '\0')
		s++;
	} else {
	    while (*s > ' ')
		s++;
	}
	if (*s != '\0')
	    *s++ = '\0';
    }
    *pac = n;
    while (n < MXNARG)
	av[n++] = nulstr;
}

void
mac_install(int ac, char **av)
{
    char   *is, *ds, *str, *ts;
    unsigned int ns, nc, bs;

#ifdef DEBUG
    if (flags_.dbflg) {
	sprintf(ctext_.msg, "mac_install: %d '%s' '%s' '%s' '%s'\n",
	    ac, av[0], av[1], av[2], av[3]);
	errout(ctext_.msg);
    }
#endif

    if (ac < 2)
	return;
    mac_undefine(ac, av);
    if (nmd >= MXNMAC) {
	errout("too many macros\n");
	return;
    }
    nc = (int)strlen(av[1]);
    if ((md[nmd].nam = (char *) malloc(nc + 1)) == NULL) {
	errout("not enough memory for macro\n");
	return;
    }
    strcpy(md[nmd].nam, av[1]);

    if (ac < 3) {
	str = ctext_.msg;	/* assumes MAXMSG >= MAXLINE */
	ns = 0;
	bs = MAXLINE;
	ds = (char *) malloc(bs);
	ds[0] = '\0';
	while (get_str(misc_.lunin, str, MAXLINE) == 0) {
	    is = str;
	    while (*is <= ' ' && *is != '\0') {
		is++;
            } if (strncmp(is, "%%%", 3) == 0) {
		is++;
            } else if (strncmp(is, "%%", 2) == 0) {
		break;
            }
	    nc = (int)strlen(is);
	    if (bs < (ns + nc + 2)) {
		bs += MAXLINE + 2;
                ts = malloc(bs);
		if (ts == NULL) {
		    errout("not enough memory for macro definition\n");
		    free(ds);
                    return;
		}
                memcpy(ts,ds,ns);
                free(ds);
                ds = ts;
	    }
	    strncpy(ds + ns, is, nc);
	    ns += nc;
	    ds[ns++] = '\n';
	    ds[ns] = '\0';
	}
	md[nmd].def = ds;
    } else {
	if ((md[nmd].def = (char *) malloc(strlen(av[2]) + 1)) == NULL) {
	    errout("not enough memory for macro definition\n");
	    return;
	}
	strcpy(md[nmd].def, av[2]);
    }
#ifdef DEBUG
    if (flags_.dbflg) {
	sprintf(ctext_.msg, "nam=%s def=%s\n", md[nmd].nam, md[nmd].def);
	errout(ctext_.msg);
    }
#endif
    nmd++;
}

void
mac_undefine(int ac, char **av)
{
    int     i, mn;

    if (ac < 2 || nmd < 1)
	return;
    if ((mn = mac_lookup(av[1])) < 0)
	return;
    free(md[mn].nam);
    free(md[mn].def);
    nmd--;
    for (i = mn; i < nmd; i++) {
	md[i].nam = md[i + 1].nam;
	md[i].def = md[i + 1].def;
    }
}

void
mac_repeat(int ac, char **av)
{
    char    repnum[20];
    int     i, mn, nr;
    double  tmp;
    static char *fn = "!repeat";

    if (ac < 2)
	return;
    if (getnum(av[1], &tmp, " \t") < 0)
	return;
    nr = (int) tmp;
    av[1] = fn;
    mac_install(ac, av);
    if ((mn = mac_lookup(fn)) < 0)
	return;
    av[0] = repnum;
    for (i = nr; i > 0; i--) {
	sprintf(repnum, "%d", i);
	mac_pubdef(mn, 1, av);
    }
    mac_undefine(ac, av);
}

void
mac_for(int ac, char **av)
{
    char    repnum[20], sn[8], *da[2];
    int     i, j, ic, mn, nr, n1, n2;
    static char *da_for[2] = {"", "!for"};

    if (ac < 2)
	return;
    mac_install(2, da_for);
    if ((mn = mac_lookup(da_for[1])) < 0)
	return;
    da[0] = repnum;
    nr = ac - 1;
    for (i = nr; i > 0; i--) {
	sprintf(repnum, "%d", i);
        if (is_seq(av[i])) {
            da[1] = sn;
            ic = indx(av[i], ':');
            n1 = atoi(av[i]);
            n2 = atoi(av[i]+ic);
            if (n2 > n1) {
                for (j = n2; j >= n1; j--) {
                    sprintf(sn,"%d", j);
	            mac_pubdef(mn, 2, da);
                }
            } else {
                for (j = n2; j <= n1; j++) {
                    sprintf(sn,"%d", j);
	            mac_pubdef(mn, 2, da);
                }
            }
        } else {
            da[1] = av[i];
	    mac_pubdef(mn, 2, da);
        }
    }
    mac_undefine(ac, av);
}

void
mac_msg(int ac, char **av)
{
    int i, n;
    double a[8];
 
    if (ac < 3)
	return;
    sprintf(ctext_.msg, "(%s) (%s) \"", av[1], av[2]);
    n = (int)strlen(ctext_.msg);
    for (i = 0; i < (ac - 3); i++)
        if (getnum(av[i + 4], &a[i], " \t") < 0)
            a[i] = 0;
    while (i < 8)
        a[i++] = 0;
    sprintf(ctext_.msg + n, av[3],
         a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
    n = (int)strlen(ctext_.msg);
    strcat(ctext_.msg + n, "\"\n");
    pubstr(ctext_.msg);
}

int
mac_lookup(char *s)
{
    int     i;

    for (i = 0; i < nmd; i++) {
	if (strcmp(s, md[i].nam) == 0)
	    return (i);
    }
    return (-1);
}

void
mac_pubdef(int mn, int ac, char **av)
{
    char   *s, *e, *s1, *s2, *s3, *s4, *ds;
    int     d, dc;

    s = md[mn].def;
#ifdef DEBUG
    if (flags_.dbflg) {
	sprintf(ctext_.msg, "mac_pubdef: mn=%d, s='%s'\n", mn, s);
	errout(ctext_.msg);
    }
#endif
    pubchr('\n');
    if (indx(s, '$') == 0) {	/* check for args */
	pubstr(s);
    } else {			/* arg substitution */
	e = s;
	d = (ac <= 10) ? ('0' + ac - 1) : '9';
	while (*e)
	    e++;
	while (e > s) {
	    s1 = e - 1;
	    s2 = e - 2;
	    s3 = e - 3;
	    s4 = e - 4;
	    if (s1 == s || *s2 != '$') {
	    	if (ac > 10 && *s3 == '$' 
	    	    && *s2 > '0' && *s2 <= ('0' + (ac - 1) / 10)
	    	    && *s1 >= '0' && *s1 <= ('0' + (ac - 1) % 10)) {
	    	    if (s4 >= s && *s4 == '$') {    /* $$dd */
		        pubchr(*s1);
		        pubchr(*s2);
		    } else {                        /* x$dd */
		        pubstr(av[(*s2 - '0') * 10 + (*s1 - '0')]);
    		        e--;
		    }
		    e--;
    	        } else {
		    pubchr(*s1);
		}
	    } else {
                for (dc=0, ds=s2; *ds=='$' && ds>=s; dc++,ds--)
                    continue;                       /* count $ */
                dc %= 2;
		if (dc != 0 && *s1 == '$') {        /* even $ */
		    pubchr('$');
		    e--;
		} else if (dc != 0 
                    && *s1 >= '0' && *s1 <= d) {    /* odd $, digit*/
		    pubstr(av[*s1 - '0']);
		    e--;
                } else {
		    pubchr(*s1);
		}
	    }
	    e--;
	}
    }
}

void
mac_getdef(int mn, char *s)
{
    strcpy(s, md[mn].def);
}

void
mac_preinst()
{
    char *av[3];
    int ac, i;
    static char *nam[1] = {
	"straline"
    };
    static char *def[1] = {
        // straline ; Slope, Yint, Lintype, X1, X2
	"p=pltype\n"
	"l=lintype\n"
	"pltype=2\n"
	"lintype=$3\n"
	"clip=yes\n"
	"ndata=1\n"
	"xdata=\n"
	"ydata=\n"
	"zdata=\n"
	"Y1=$1*$4+$2\n"
	"Y2=$1*$5+$2\n"
	"($4) (Y1)\n"
	"($5) (Y2)\n"
	"plot\n"
	"lintype=(l)\n"
	"pltype=(p)\n"
    };
    static int nm = 1;

    ac = 3;
    av[0] = "%define";
    for (i = 0; i <nm; i++) {
        av[1] = nam[i];
        av[2] = def[i];
	mac_install(ac, av);
    }

}

void
mac_clear(void)
{
    int     i;

    for (i = 0; i < nmd; i++) {
        free(md[i].nam);
	free(md[i].def);
    }
    nmd = 0;
    mac_preinst();
}
