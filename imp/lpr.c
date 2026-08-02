/* lpr.c */

#include <stdio.h>

unsigned int strlen();
extern FILE *prn;

void
lpr(s, n)
char   *s;
unsigned int n;
{
    write(fileno(prn), s, n);
}

void
lprs(s)
char   *s;
{
    lpr(s, strlen(s));
}

void
lprfl()
{
    (void) fflush(prn);
}

