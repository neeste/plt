/*
 *
 * 	getmsg (instr)
 *
 *	DATE OF FIRST VERSION:		31-Mar-80
 *	DATE OF CURRENT VERSION:	8-Sep-88
 *
 *	PURPOSE:	PLTFIL routine to obtain message lines
 *
 *	USAGE:		GETMSG (instr)
 *
 *			Where	instr = character array with first message line
 *
 *	Functions called:
 *			fprintf, getdat, get_str, get_tty, indx,
 *			putmsg, skipwhite, strchr, strcpy
 *
 *	Called by:
 *			proces
 *
 *	PROGRAMMER:	Mike Collins / Eric Javel / Matt Prucka
 *
 ************************************************************
 */

#include <string.h>
#include "plt.h"
#include "pltblk.h"
#include "fort.h"

#define CNULL '\0'
#define EOL '\025'
#define TAB '\011'

char   *skipwhite(char *);
int     getdat(char *, double *, int);
int     get_str(FILE *, char *, int);
int     get_tty(char *, char *, int);
int     indx(char *, int);
void    putmsg(char *, int);

void
getmsg(char *instr)
{
    int     term, openquote, endquote, nline, ier;
    char   *ps;
    char   *prompt = " Enter message line. Terminate with blank line: ";
    double  col[2];

/* Find first double quote mark */
    if ((openquote = indx(instr, '"')) == 0)
	return;

/*  Skip blank leading blanks */

    ps = skipwhite(instr);

/* Check for (mxllc,myllc) */

    col[0] = param_.mxllc;
    col[1] = param_.myllc;
    (void) getdat(ps, col, 2);
    param_.mxllc = (float) col[0];
    param_.myllc = (float) col[1];

/*
 *  Get the first line of the message and
 *  eliminate the leading quotation mark.
 */
    if (instr[openquote] != '*' || !misc_.in_tt) {
	ps = instr + openquote;
	term = FALSE;
    } else {
	ps = instr;
	term = TRUE;
	if (get_tty(prompt, ps, MAXLINE) < 0)
	    return;
	if (ps[0] == CNULL)
	    return;
    }
/*
 * Get more lines until a delimiting quotation mark is found,
 * output each line as its read.
 */
    for (nline = 0;;) {
	endquote = indx(ps, '"');
	if (term || (endquote > 1 && ps[endquote - 2] == '\\'))
	    endquote = 0;
	else if (endquote > 0)
	    ps[endquote - 1] = CNULL;
#ifdef DEBUG
	if (flags_.dbflg) {
	    sprintf(ctext_.msg,
		"getmsg: %d '%s'\n", nline, ps);
	    errout(ctext_.msg);
	}
#endif
	putmsg(ps, nline++);
	ps = instr;
	*ps = CNULL;
	if (endquote > 0)
	    break;
	if (term)
	    ier = get_tty(prompt, ps, MAXLINE);
	else
	    ier = get_str(misc_.lunin, ps, MAXLINE);
	if (ier < 0)
	    break;
	if (term && *ps == CNULL)
	    break;
	if (!term && param_.echo != 0.0)
	    fprintf(stderr, " %s\n", ps);
    }
    putmsg(ps, nline);
}
