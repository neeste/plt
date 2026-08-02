/* imp2fx.c */
/* Translates impress language into EPSON FX graphics commands. */

#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>		/* required for setmode */
#include <string.h>
#include <math.h>
#include <io.h>
#include <stdlib.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif

FILE   *in;
FILE   *prn;
int     prdpi = 120;
int     verbose = 0;
int     band = 0;
int     nband = 11;
int     xband = 0;
int     xpage = 0;
int     xmin = 0;
int     ymin = 0;
int     xmax = 0;
int     ymax = 0;
int     xoff = 0;
int     yoff = 0;
long    vecpp = 0;
long    vecpb = 0;

static char infile[40] = "";
static int page = 1;
static int gmset = 0;
static long place1 = 0;
static long place2 = 0;

int     imp_scan();
void    cant();
void    process();
void    doplot();
void    prnsetup();
void    prndump();
void    prnrestore();
void    clear_page();
void    clear_band();
void    tpcl();
void    make_spot();
void    spotset();
void    spotclr();
void    usage();

/* main - parse command line options and open needed files */

void
main(ac, av)
char  **av;
{
    prn = stdout;
    in = stdin;
    if (ac > 1) {
	while (av[1][0] == '-') {
	    switch (av[1][1]) {
	    case 'h':
		usage();
		break;
	    case 'o':
		if ((prn = fopen(av[2], "wb")) == NULL)
		    cant(av[2]);
                ac--;
                av++;
		break;
	    case 'q':
		prdpi = 240;
		break;
	    case 'r':
		prdpi = atoi(av[1] + 2);
		break;
	    case 'v':
		verbose = 1;
		break;
	    case 'x':
		xoff = (int) (atof(av[2]) * 300);
                ac--;
                av++;
		break;
	    case 'y':
		yoff = (int) (atof(av[2]) * 300);
                ac--;
                av++;
		break;
	    }
	    ac--;
	    av++;
	}
    }
    if (ac <= 1 && isatty(fileno(in)))
	usage();
    prnsetup();
    if (--ac == 0) {
	(void) setmode(fileno(stdin), (int) O_BINARY);	/* do binary input */
	process();
    } else
	while (ac > 0) {
	    (void) strcpy(infile, av[1]);
	    if ((in = fopen(infile, "rb")) == NULL) {
		if (strchr(infile, '.') == NULL)
		    (void) strcat(infile, ".imp");
		if ((in = fopen(infile, "rb")) == NULL)
		    cant(infile);
	    }
	    if (verbose)
		(void) fprintf(stderr, "file: %s\n", infile);
	    process();
	    (void) fclose(in);
	    ac--;
	    av++;
	}
    if (gmset)
	prnrestore();
    (void) fclose(prn);
    exit(0);
}

void
process()
{
    int     status;

    clear_page();
    vecpp = 0;
    vecpb = 0;
    for (band = 1; band <= nband; band++) {
	if (band < nband)
	    xmax = xband;
	else
	    xmax = xpage - xband * (nband - 1);
	status = imp_scan();
	if (vecpp > 0) {
	    doplot();
	} else if (status == EOF) {
	    return;
	} else {
	    band = 0;
	    place1 = place2;
	    place2 = 0;
	}
    }
}

/* doplot - output a band of graphics & rewind the file */

void
doplot()
{
    if (verbose) {
	if (band == 1)
	    (void) fprintf(stderr, "  page %d: %ld vectors\n", page, vecpp);
	if (nband > 1)
	    (void) fprintf(stderr, "    band %d: %ld vectors\n", band, vecpb);
    }
    prndump();
    gmset = 1;
    vecpp = 0;
    vecpb = 0;
    if (band < nband) {
	(void) fseek(in, place1, 0);
	clear_band();
    } else {
	band = 0;
	place1 = place2;
	place2 = 0;
	clear_page();
	page++;
    }
}

/* save_place - save current place in input file for later return */

int
save_place()
{
    if (place2 == 0)
	place2 = ftell(in);
    return (1);
}

/* cant - abort program when a needed file can't be opened */

void
cant(file)
char   *file;
{
    (void) fprintf(stderr, "can't open '%s'\n", file);
    exit(0);
}

/* usage - one screen suammary of how to use the program */

void
usage()
{
    (void) printf("\n\n");
    (void) printf("program name: IMP2FX, version 1.3\n\n");
    (void) printf("Reads a file of impress commands and outputs EPSON FX graphics to 'stdprn.'\n");
    (void) printf("When no file name arguments are present, reads impress from 'stdin'.\n\n");
    (void) printf("usage: imp2fx [options] impress_file \n\n");
    (void) printf("options:\n");
    (void) printf("    -h   	print usage information\n");
    (void) printf("    -o F 	put graphic codes in file F instead of stdprn\n");
    (void) printf("    -q   	set resolution to 240 dpi [default = 120 dpi]\n");
    (void) printf("    -rN   	set resolution to N dpi, N=120,240\n");
    (void) printf("    -v   	verbose mode\n");
    (void) printf("    -x N 	x page-offset (inches)\n");
    (void) printf("    -y N 	y page-offset (inches)\n");
    (void) printf("\n\n");
    exit(0);
}
