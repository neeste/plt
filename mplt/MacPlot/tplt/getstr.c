/* get_str.c */

#include "plt.h"
#include "pltblk.h"
#include "cincl.h"

#define ngetc(lun,i)	(ncpb[i] ? pubbuf[i][--ncpb[i]] : getc(lun))
#define MAXPUBC		(1<<22)

void    pubchr(int);
void    pubstr(char *);

static char *pubbuf[MAXINCL];
static unsigned int pubsiz[MAXINCL] = {0}, ncpb[MAXINCL] = {0};

/*
 * get_str - get a string
 *	Functions called: getc
 *	Called by: getmsg get_tty nxtlin
 */
int
get_str(FILE *lun, char *lin, int nc)
{
    register char *pend, *ps;
    register int c, d;

    ps = lin;
    pend = lin + nc - 1;
    while ((c = ngetc(lun, cincl_.inclev)) != '\n' && c != '\r' && c != EOF) {
	if (ps < pend) {
	    *ps++ = c;
	}
    }
    if (c == '\n' || c == '\r') {
	d = ngetc(lun, cincl_.inclev);
	if ((c == '\r' && d != '\n') || (c == '\n' && d != '\r') ) {
	    pubchr(d);
	}
    }
    for (ps--; lin <= ps && (*ps <= ' '); --ps)
	continue;		/* remove any trailing whitespace */
    *(++ps) = '\0';
    return (((ps <= lin && c == EOF) ? EOF : 0));
}

/*
 * get_tty - get a string from the console device with prompt
 *	Functions called: fprintf fflush get_str
 *	Called by: getmsg indata namfil stnmpm sttxpm
 */
int
get_tty(char *prompt, char *lin, int nc)
{
    if (!misc_.in_tt)
	return (EOF);
    fputs(prompt, stderr);
    fflush(stderr);
    return ((int)get_str(stdin, lin, nc));
}

void
pubstr(char *s)
{
    char   *e;

    e = s;
    while (*e)
	e++;
    while (e > s)
	pubchr(*--e);
}

void
pubchr(int c)
{
    int i;
    
    i = cincl_.inclev;
    if (pubsiz[i] == 0) {
	pubsiz[i] = MAXLINE;
	pubbuf[i] = (char *) malloc(pubsiz[i]);
    } else if (ncpb[i] >= MAXPUBC) {
	errout("Excessive macro push back\n");
	return;
    } else if (pubsiz[i] <= ncpb[i]) {
	pubsiz[i] += MAXLINE;
	pubbuf[i] = (char *) realloc(pubbuf[i], pubsiz[i]);
    }
    if (pubbuf[i] == NULL) {
	errout("Not enough memory for macro push back\n");
	pubsiz[i] = 0;
	return;
    }
    pubbuf[i][ncpb[i]++] = c;
}

void
pubclr(void)
{
    int i;
    for (i = 0; i < MAXINCL; i++)
	ncpb[i] = 0;
}
