/* tstout.c - test program for libout */

#include "output.h"

float   xm[]= {5.0F};
float   ym[]= {5.0F};
float   xa[]= {0.1F, 0.1F, 10.4F, 10.4F, 0.1F, 1.1F, 1.1F, 9.4F, 9.4F, 1.1F};
float   ya[]= {0.1F, 7.9F,  7.9F,  0.1F, 0.1F, 1.0F, 7.0F, 7.0F, 1.0F, 1.0F};

void
tstout(int ac, char *av[])
{
    int idev;

    idev = ac > 1 ? atoi(av[1]) : defdev();
    set_title("tstout");
    (void) opnout(idev);	/* open output */
    setpgb(0);
    setfgci(15);		/* set foreground color to white */
    setbgci(1);			/* set background color to dark blue */
    setfnci(4);			/* set "fountain" color to light blue */
    setwidestroke(1);		/* set wide strokes (on PC) */
    setmkt(30);			/* set marker type */
    setmks(1.);			/* set marker size */
    setmkci(14);		/* set marker color index */
    marker(1, xm, ym);		/* plot marker */

    setchh(1.);			/* set character height */
    setchan(10.);		/* set character slant angle */
    text(3., 3., "Hello!");	/* plot test */

    setfais(3);			/* set fill area interior style */
    setfasi(10);		/* set fill area style index */
    setfaci(11);		/* set fill area color index */
    area(10, xa, ya);		/* hatch area */

    setfais(0);			/* set fill area interior style */
    setfasi(0);			/* set fill area style index */
    setfaci(3);			/* set fill area color index */
    area(5, xa, ya);		/* outline area */
    area(5, xa + 5, ya + 5);	/* outline area */

    clsout();			/* close output */
}

#ifdef MAIN
int
main(int ac, char *av[])
{
fprintf(stderr, "main\n");
    tstout(ac, av);
    return(0);
}
#endif /* MAIN */
