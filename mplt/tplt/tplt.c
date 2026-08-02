/*
 * tplt.c - main routine for PLT program
 */

#include <string.h>
#include "plt.h"
#include "pltblk.h"
#include "fort.h"
#include "cincl.h"
#include "version.h"

#ifndef _MSC_VER
# ifdef linux
#   define  OS      "LINUX"
# else
# ifdef __MACH__
#   define  OS      "MAC"
# else
#   ifdef mgw
#     define  OS      "MinGW"
void    win_mesg(char *, char *);
#   else
#     define  OS      "DOS"
#   endif // mgw
# endif // MAC
# endif // linux
#   define  IN_TT   isatty(fileno(stdin))
#include <unistd.h>
#else
# ifdef MSDOS
#   define  OS      "MSDOS"
#   define  IN_TT   _isatty(_fileno(stdin))
# endif
# ifdef WIN32
void    win_mesg(char *, char *);
#   define  OS      "WIN32"
#   define  IN_TT   0
# endif
#endif //_MSC_VER
#define free_zero(x)	{if(x){free(x);x=0;}}

/* define globals  */
int     inq_page(void);
int     noext(char *);
int     setup(int, int, int);
int     get_str(FILE *, char *, int);
int     iscan(char *[], int, char *);
int     cnglun(void);
void    leave(void);
void    version(void);
void    usage(void);
void    fatal(int);
void    newext(char *, char *);
void    proces(void);
void    pubchr(int);
void    pubstr(char *);
void    set_page(int);
void    merge(void);
void    opninc(char *);

double  inch_unit = 1.0;
int     firstf = 1;
int     incldf = 0;
int     maxnp = 0;
int     ndatp = 0;
int     nsavp = 0;
float  *xdat = 0, *ydat = 0, *zdat = 0;
float  *xsav = 0, *ysav = 0, *zsav = 0;
struct param param_ = {0};
struct flags flags_ = {0};
struct ctext ctext_ = {{0}};
struct cincl cincl_ = {0};
struct misc misc_ = {{0}};
struct axes axes_ = {0};
struct size size_ = {0};


void
tplt(int argc, char **argv)
{
    char   *cav;
    int     iecho = 0, idevice = 1, itxprec = TXPRDEF;

    inch_unit = 1.0;
    firstf = 1;
    maxnp = 0;
    ndatp = 0;
    xdat = ydat = zdat = NULL;

    idevice = defdev();		/* Get default device number */
    misc_.lunout = stdout;	/* Logical unit no. for output */
    misc_.ifrom = inq_page();	/* default start page = 1 */ 
    misc_.ito = 999;		/* default end page = 999 */
    misc_.pgprmt = -1;
    misc_.pgbord = -1;
    misc_.numpen = -1;
    misc_.colors = TRUE;
    misc_.in_tt = IN_TT;
    misc_.pltyet = FALSE;
    flags_.newpag = TRUE;
    strcpy(misc_.filonm, "stdout");

    while (argc > 1) {
        cav = argv[1];
	if (cav[0] == '-' && cav[1] != '\0') {
	    switch (cav[1]) {
	    case 'b':
		misc_.pgbord = TRUE;
		break;
	    case 'c':
		if (cav[2] == 'm')
		    inch_unit = 2.54;
		else if (cav[2] == '\0')
		    misc_.colors = TRUE;
		break;
	    case 'd':
		flags_.dbflg = TRUE;
		break;
	    case 'e':
		iecho = TRUE;
		break;
	    case 'h':
		usage();
		break;
	    case 'i':
		incldf = 1;
                if (argc > 2) {
                    strncpy(misc_.filonm, argv[2], NAMESIZE);
                    argc--;
                    argv++;
                } else {
                    misc_.filonm[0] = 0;
                }
		break;
	    case 'm':
		if (cav[2] == 'm')
		    inch_unit = 25.4;
		else if ('0' <= cav[2] && cav[2] <= '9')
		    idevice = cav[2] - '0';
		break;
	    case 'n':
		if (cav[2] == 'b')
		    misc_.pgbord = FALSE;
		else if (cav[2] == 'c')
		    misc_.colors = FALSE;
		else if (cav[2] == 'q')
		    misc_.pgprmt = FALSE;
		else if (cav[2] == 's')
		    misc_.slomod = FALSE;
		break;
	    case 'o':
		if ('1' <= cav[2] && cav[2] <= '9')
		    misc_.ifrom = misc_.ito = cav[2] - '0';
		break;
	    case 'p':
		misc_.numpen = 1;
		break;
	    case 'q':
		misc_.pgprmt = TRUE;
		break;
	    case 'r':
		orient(1);
		break;
	    case 's':
		misc_.slomod = TRUE;
		break;
	    case 't':
		switch (cav[2]) {
		case 'c':
		    itxprec = TXPRCHR;
		    break;
		case 'e':
		    itxprec = TXPREXT;
		    break;
		case 'h':
		    itxprec = TXPRSTR;
		    break;
		case 's':
		    itxprec = TXPRSTK;
		    break;
		}
		break;
	    case 'v':
		version();
		fatal(0);
		return;
	    }
#ifdef MAIN	    /* Allow parameters setting in command line */
	} else if (strchr(cav, '=') != NULL) {
	    pubstr(cav);
	    pubchr('\n');
#endif
	} else {
	    if (cav[0] == '-') {
		strcpy(misc_.filinm, "stdin");
		misc_.lunin = stdin;
	    } else {
		strcpy(misc_.filinm, cav);
		misc_.lunin = fopen(misc_.filinm, "rt");
	    }
            if (misc_.lunin == NULL && noext(misc_.filinm)) {
                newext(misc_.filinm, ".plt");
                misc_.lunin = fopen(misc_.filinm, "rt");
            }
            if (misc_.lunin == NULL) {
                sprintf(ctext_.msg, "plt: can't open file %s\n", misc_.filinm);
                fatal(1);
                return;
            }
            if (incldf) {
                merge();
                incldf = 0;
                break;
            }
            if (!setup(iecho, idevice, itxprec)) {
                if (misc_.lunin != stdin)
                    fclose(misc_.lunin);
                break;
            }
            proces();
            if (misc_.lunin != stdin) {
                fclose(misc_.lunin);
		misc_.lunin = NULL;
	    }
        }
        argc--;
        argv++;
    }
    leave();
}

void
version(void)
{
    sprintf(ctext_.msg, "PLT for %s %s\n", OS, REV);
}

void
usage(void)
{
    printf("PLT for %s %s\n", OS, REV);
    printf("usage: plt [-options] pltfile ...\n");
    printf("options:\n");
    printf(" -b   = page border\n");
    printf(" -c   = colors\n");
    printf(" -cm  = centimeters\n");
    printf(" -d   = debug\n");
    printf(" -e   = echo\n");
    printf(" -h   = print help\n");
    printf(" -i   = merge include files\n");
    printf(" -mm  = millimeters\n");
    printf(" -mN  = output mode N\n");
    printf(" -nq  = no page prompt\n");
    printf(" -oN  = begin with page N\n");
    printf(" -r   = rotate EPS\n");
    printf(" -v   = print version\n");
}

void
free_dat(void)
{
    free_zero(xdat);
    free_zero(ydat);
    free_zero(zdat);
    free_zero(xsav);
    free_zero(ysav);
    free_zero(zsav);
}

void
leave(void)
{
    free_dat();
    clsout();
#ifndef MAIN
    if (firstf)
	fprintf(stderr, "usage: plt [-options] pltfile ...\n");
#endif
}

void
fatal(int ec)
{
    free_dat();
    clsout();
#ifdef WIN32
    win_mesg(ec ? "Fatal Error" : "PLT version", ctext_.msg);
#else /* WIN32 */
    fprintf(stderr, "\n%s", ctext_.msg);
    fprintf(stderr, "%s %s\n%s\n", COPY, BTNRH, RIGHTS);
#endif /* WIN32 */
    exit(ec);
}

int
noext(char *f)
{
    char   *s;

    s = f;
    while (*s != '\0')		/* locate end of filename */
	s++;
    while (*s != '\\' && *s != '/' && s > f)	/* backup to start of name */
	s--;
    while (*s != '\0' && *s != '.')	/* locate end of name */
	s++;
    return (*s == '\0');	/* return true if no extension */
}

void
newext(char *f, char *e)
{
    char   *s;

    s = f;
    while (*s != '\0')		/* locate end of filename */
	s++;
    while (*s != '\\' && *s != '/' && s > f)	/* backup to start of name */
	s--;
    while (*s != '\0' && *s != '.')	/* locate end of name */
	s++;
    while (*e != '\0')		/* copy extension */
	*s++ = *e++;
    *s = '\0';
}

void
merge(void)
{
    char instr[MAXLINE];
    int j, k;
    static char *ss = ";;;;---- ";
    static char *kw[2] = {"CALL", "INCL"};
    static int nk = sizeof(kw) / sizeof(char *);

    misc_.lunout = fopen(misc_.filonm, "wt");
    if (misc_.lunout == NULL) return;
    cincl_.inclev = 0;		/* Set include level to 0 */
    cincl_.inc_file[0] = misc_.lunin;	/* Set include file pointer */
    for(;;) {
        j = (int) get_str(misc_.lunin, instr, NAMESIZE);
        if (j == EOF) {
            if (cincl_.inclev > 0) {
                cnglun();
                fputs(ss, misc_.lunout);
            } else {
                break;
            }
        }
        k = iscan(kw, nk, instr);
        if (k) {
           j = cincl_.inclev;
           opninc(instr);
            if (cincl_.inclev > j) {
                fputs(ss, misc_.lunout);
            }
        }
        fprintf(misc_.lunout, "%s\n", instr);
    }
    fclose(misc_.lunout);
}

#ifdef MAIN
int
main(int argc, char **argv)
{
    tplt(argc, argv);
    return(0);
}
#endif
