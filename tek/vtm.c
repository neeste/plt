/* vtm - Enter vt100 mode */

#include <stdio.h>

int
main(int av, char **ac)
{
    fputs("\030",stdout);	/* ^X for mskermit */
    fputs("2",stdout);	/* hirez: enter ansi mode */
    fputs("\"0g",stdout);	/* retrographics: enter transparent mode */
    fputs("  ",stdout);	/* Erase leftovers */
    return(0);
}
