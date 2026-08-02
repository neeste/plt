/*
 *
 *     subroutine proces
 *
 *	DATE OF  FIRST  VERSION:	21-jun-83
 *	DATE OF  CURRENT  VERSION:	10-oct-90       STN
 *
 *	PURPOSE:	read lines from a plt file and call indicated functions
 *
 *	Functions called:
 * 			cnglun, errout, getmsg, indata, nxtlin, pltkey, setprm,
 *			sprintf
 *
 *	Called by:
 *			main
 *
 *	PROGRAMMER:	Matt Prucka
 *
 **********************************************************************
 */
#include "plt.h"
#include "pltblk.h"
#include "fort.h"

/*
 * Command types set by NXTLIN
 */
#define COMMENT 0
#define DATUM 1
#define KEY 2
#define PARAM 3
#define MESSAGE 4
#define YES 1
#define NO 0

int     cnglun(void);
int     indata(char *);
int     mordat(void);
int     nxtlin(char *);
int     inq_break(void);
void    getmsg(char *);
void    leave(void);
void    pltkey(char *);
void    setprm(char *);

extern int maxnp;
extern int ndatp;
extern float *xdat, *ydat, *zdat;

static char instr[MAXLINE];

void
proces(void)
{

/*
 * Do the plot by reading input lines and processing them
 */
    ndatp = 0;
    misc_.npkept = 0;
    misc_.lftovr = 0;
    misc_.inow = 1;
    while (misc_.inow <= misc_.ito) {
	flags_.finish = FALSE;

	switch (nxtlin(instr)) {
	case COMMENT:		/* Do nothing for comment */
	    break;
	case DATUM:		/* Data value */
	    while (indata(instr) == 1) {
		if (ndatp >= maxnp && mordat()== 0) {
		    errout("WARNING: Number of data points exceeds buffer size.\n");
		    pltkey("plot");	/* do "plot" to clear data list */
		}
	    }
	    break;
	case PARAM:		/* Parameter alteration */
	    setprm(instr);
	    break;
	case MESSAGE:		/* Message */
	    getmsg(instr);
	    break;
	case EOF:		/* End of file */
	    pltkey("fini");	/* do "finish" */
	    break;
	case KEY:
	    pltkey(instr);	/* Keyword, maybe. */
	    break;
	default:
	    sprintf(ctext_.msg, "Unknown command: %s\n", instr);
	    errout(ctext_.msg);
	}
	if (flags_.exit == TRUE) {
	    setpgp(0);		/* exit with no prompt */
	    leave();
	} else if (flags_.finish == TRUE) {	/* end of current page ? */
	    if (cnglun() == EOF) {	/* end of "main" file ? */
		pltkey("plot");		/* do "plot" */
		break;
	    }
	}
        if (inq_break())
	    break;
    }
}
