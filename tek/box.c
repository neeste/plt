/* box.c */

#include <stdio.h>

#define ADUPI 390.		/* addressable device units per inch */
#define ANSI 60
#define BELL '\007'
#define CLEAR 74
#define CNTRLX 24
#define CONSI 5
#define CR  '\r'
#define EB0 96
#define EOT '\004'
#define ESC '\033'
#define FF  '\f'
#define GS  '\035'
#define HOME 72
#define HIX0 32
#define HIY0 32
#define LF '\n'
#define LOX0 64
#define LOY0 96
#define CNULL '\0'
#define RET  '\r'
#define SUB '\032'
#define SYNC '\027'
#define US  '\037'

#define	MAXSTR 79
#define	TTY 1

main(argc, argv)
    int argc;
    char *argv[];
{
    pdopen();
    pdclose();
}

/* pdopen - open the plotting device */

pdopen()
{
    tkm();		/* initially clear screen */
    pbox();		/* draw box */
}

/* pmove - move pen to (ix,iy) */

pmove(ix,iy)
    int ix, iy;
{
    static int dev = TTY, gs = GS;
    putc(GS, stdout);
    pdraw(ix, iy);
}

/* pdraw - draw a line segment from current location to (ix,iy) */

pdraw(ix, iy)
    int ix, iy;
{
    char hix, hiy, lox, loy;
    static char oldhix = CNULL, oldhiy = CNULL, oldloy = CNULL;
    int x,y;
    int dev = TTY;

    x = (ix&07777)*0.999;
    y = (iy&07777)*0.999;
    hiy = HIY0 + y/128;			/* hiy */
    loy = LOY0 + (y%128)/4;		/* loy */
    hix = HIX0 + x/128;			/* hix */
    lox = LOX0 + (x%128)/4;		/* lox */
    if(hiy != oldhiy)
	putc(hiy, stdout);
    if(loy != oldloy || hiy != oldhiy || hix != oldhix)
	putc(loy, stdout);
    if(hix != oldhix )
	putc(hix, stdout);
    putc(lox, stdout);
    oldhiy = hiy;
    oldloy = loy;
    oldhix = hix;
}

/* ppage - erases screen and home */

ppage()
{
    pwait();	/* wait for carriage return from keyboard */
    pbox();	/* erase and draw box */
}

/* ppen - does nothing */

ppen(bpen)
    int bpen;
{
    return;
}


/* pdclose */

pdclose()
{
}

/* pbox - erase and draw box */

pbox()
{
    int ix, iy;
    ix = 10.5*ADUPI;	/* 10.5 by 8 inches */
    iy = 8.0*ADUPI;

    perase();
    pmove( 0, iy);
    pdraw( 0, iy);
    pdraw(ix, iy);
    pdraw(ix, 0);
    pdraw( 0, 0);
    pdraw( 0, iy);
    pmove( 0, iy);
    fflush(stdout);
}

/* pwait - wait for a carriage return from keyboard */
pwait()
{
}

/* tkm - put vt-100 into retrographics vector mode */

tkm()
{
    putstr("<");		/* ansi */
    putstr("[2J");		/* clear */
    putstr("");		/* graphics */
    putstr("");		/* erase */
}

/* vtm - put retrographics or hirez terminal back into VT mode */

vtm()
{
    putstr("");		/* erase graphics screen */
    putstr("2");		/* hirez to ansi */
    putstr("\"0g  ");	/* retrographics to transpanent mode */
}

perase()
{
    int i;

    putstr("\0");			/* erase */
    for(i = 0; i < 100; i++)		/* short delay */
	putc(SYNC, stdout);
}

/* putstr */

putstr(pstr)
    char *pstr;
{
    char *pend;

    for(pend = pstr+MAXSTR; pstr < pend && *pstr != CNULL; pstr++) {
	putc(*pstr, stdout);
    }
}
