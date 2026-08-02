/* tekcat.c */

#include <stdio.h>
#include <stdlib.h>

#define OFF   0
#define ON    1
#define NULC  '\0'

void pdclose();
void pdopen();
void tekcvt();

extern int     plotterstatus;
extern int     penchange;
extern int     onepen;
extern FILE   *tekfp;

/*
 * tekcat - catenate and convert tek files
 */

#define ARGSIZ 80L

void
tekcat(int argc, char **argv)
{
    int     nfiles = 0;

    while (0 < --argc) {
	argv++;
	if (argv[0][0] == '-') {
	    if (argv[0][1] == NULC) {
		;
	    } else if (argv[0][1] == 'p') {
		penchange = OFF;
		if (argv[0][2] == NULC) {
		    onepen = '1';
		} else {
		    onepen = argv[0][2];
		}
		continue;
	    } else {
		continue;
	    }
	} else {
	    tekfp = fopen(*argv, "r");
	    if (tekfp == NULL) {
		fprintf(stderr, "can't open %s\n", *argv);
		exit(1);
	    }
	}
	nfiles++;
	pdopen();
	tekcvt();
	pdclose();
    }
    if (nfiles == 0) {		/* if no file names, read from stdin */
	tekfp = stdin;
	pdopen();
	tekcvt();
	pdclose();
    }
    exit(0);
}

