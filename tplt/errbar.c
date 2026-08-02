/*
 * errbar - draw the error bars
 *
 * 	Functions called:
 *		    linetp, setplt
 *
 *	Called by:
 *		    histo, linplt, rect
 *
 */

void    linetp(double, double, double, double);
void    setplt(int, int);

void
errbar(double xp, double yp, double top, double bot, double eb, double eg, double en)
{

    setplt(0, 0);		/* set solid line type */

    if (en > 0 || en < -1) {	/* upper bar */
	linetp(xp, yp + eg, xp, top);
	linetp(xp - eb, top, xp + eb, top);
    }
    if (en < 0 || en > 1) {	/* lower bar */
	linetp(xp, yp - eg, xp, bot);
	linetp(xp - eb, bot, xp + eb, bot);
    }
}
