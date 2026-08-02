/* clr - Clear graphics screen */

#include <stdio.h>
#ifndef TSX
#define puttek(s)	fputs(s, stdout)
#endif

int
main(int ac, char **av)
{
#ifdef TSX
    if (tsxenv())
	puttek("\035M");	/* set TSX transparent output mode */
#endif
    puttek("\033\f\0332\033\"0g");
    puttek("\b\b\b\b    \b\b\b\b");
    puttek("\033[?38l");	/* put kermit in text mode */
    puttek("\b\b\b\b    \b\b\b\b");
    return(0);
}

#ifdef TSX
static void
puttek(s)
char   *s;
{
    static char dc1 = 17, dc3 = 19, ctrlc = 3;
    static int *ttrs = 0, *ttrb = 0, *ttts = 0, *tttb = 0;
    static int ttrcsr = 0, tttcsr = 0;
    int     cntrl, c, addr, ofst;

    if (tsxenv()) {
	fputs(s, stdout);
    } else {
	if (ttrs == 0) {
	    /*
	       Set up I/O register addresses for direct console output under
	       RT-11. 
	    */
	    ofst = 0304;
	    addr = gval(ofst);
	    ttrs = (int *) addr;
	    ttrb = ttrs + 1;
	    ttts = ttrb + 1;
	    tttb = ttts + 1;
	}
	/* Save transmit & receive status */
	ttrcsr = *ttrs;
	tttcsr = *ttts;
	/* Disable interrupts. */
	*ttrs = 0;
	*ttts = 0;
	/* transfer bytes from buffer to console output register */
	cntrl = 1;
	while (*s != '\0') {
	    if ((*ttrs & 0200) != 0) {
		c = (*ttrb & 0177);
		if (c == dc1)
		    cntrl = 1;
		else if (c == dc3)
		    cntrl = 0;
		else if (c == ctrlc)
		    break;
	    }
	    if (cntrl == 1) {
		while ((*ttts & 0200) == 0)
		    continue;
		*tttb = *s++;
	    }
	}

	/* Restore transmit & receive status */
	*ttrs = ttrcsr;
	*ttts = tttcsr;
	c = *ttrb;
    }
}
#endif
