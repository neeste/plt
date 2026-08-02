/* tekhp.c */

#include <stdio.h>
#include <stdlib.h>
#include "arccom.h"
#include "tek.h"

/*
# tekhp - filters files from plt and sends to hp-7470 plotter
#
#
#	date of   first   version:  26-Jun-84
#
#
#**********************************************************************
 */

FILE *ttyfp;
FILE *hp7475fp;


main(argc, argv)
    int argc;
    char *argv[];
{
    tekcat(argc, argv);
    exit(0);
}


/*
# pdopen
 */
pdopen()
{
    arccom_.segmin = 1.;		/* # minimum arc segment length */
    arccom_.arcsmo = .0625;		/* # arc smoothness */

    if ( (ttyfp = fopen("/dev/tty", "w+")) == NULL) {
	fprintf(stderr, "can't write to device /dev/tty\n");
	exit(1);
    }

    if ( (hp7475fp = fopen("plt.hpgl", "w+")) == NULL) {
	fprintf(stderr, "can't write to device /dev/hp7475\n");
	exit(1);
    }
    putstr("IN;IP 0,0,10300,7650;");
    putesc(".I81;;17:");	/* # set handshake mode xon/xoff */
    putesc(".N;19:");		/* # set handshake mode xon/xoff */
    putstr("VS10;");		/* # set pen velocity 10 cm/s */
}

/*
# pdclose
*/
pdclose()
{
    putstr("PU;SP;");
    fflush(hp7475fp);
}

/*
# pmove -
 */
pmove(ix,iy)
    int ix, iy;
{
    fflush(hp7475fp);		/* may not need could just slow things down */
    ppos("PU",ix,iy);
}

/*
# pdraw -
 */
pdraw(ix,iy)
    int ix, iy;
{
    ppos("PD",ix,iy);
}

#define SX 2.5
#define SY 2.5

#define NINT(a) ( (int) (((a) < 0.0) ? (a) - 0.5 : (a) + 0.5))
/*
# ppos - position pen
 */
ppos(pstr,ix,iy)
    char pstr;
    int ix, iy;
{
/*
    write(2,200)str,nint(sx*ix),nint(sy*iy)
    200 format(a,i5,',',i5,';')
 */
    fprintf(hp7475fp, "%s%5d,%5d;\n", NINT(SX*ix), NINT(SY*iy));
}

static int hpcom_ipen = '0';	/* '0' is a guess since never set */
/*
# ppage - new page
 */
ppage()
{
    putstr("PU;SP;");
    fflush(hp7475fp);		/* may slow things down */
    pwait();
    ppen(hpcom_ipen);
}

/*
# ppen - change pens
 */
ppen(num)
    int num;
{
    char str[7];

    sprintf(str, "PU;SP%c;", num);
    putstr(str);
    hpcom_ipen = num;
}

/*
# pwait - wait for a carriage return from keyboard
 */
pwait()
{
    char junk;

    fflush(hp7475fp);
    fprintf(ttyfp, "\07Please change paper and hit return\n");
    fflush(ttyfp);
    fseek(ttyfp, 0L, 0);
    junk = getc(ttyfp);
    fseek(ttyfp, 0L, 0);
}

/*
# putstr - put a string
 */
putstr(pstr)
    char *pstr;
{
/*
    write(2,200)str
    200 format(a)
 */
    fprintf(hp7475fp, "%a\n", pstr);
}

/*
# putesc - send escape string to plotter
 */
putesc(pstr)
    char *pstr;
{
/*
    write(2,200)char(27),str
    200 format(a,a)
 */
    fprintf(hp7475fp, "%s\n", pstr);
}
