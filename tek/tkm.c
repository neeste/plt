/* tkm - Enter tektronix mode */

#include <stdio.h>

int
main(int ac, char **av)
{
    fputs("<",stdout);	/* retrographics: enter ansi mode */
    fputs("",stdout);		/* enter tektronix mode */
    fputs("0",stdout);	/* retrographics: small character size */
    return(0);
}
