/* generate test data */

#include <stdio.h>

#ifdef MSDOS
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define bopen(fn)	open(fn, O_CREAT|O_BINARY|O_WRONLY, S_IWRITE|S_IREAD)
#else
#include <sys/file.h>
#define bopen(fn)	open(fn, O_CREAT|O_TRUNC|O_WRONLY, 0644)
#endif

static struct {
    short int magic, nbpv, nvpp, npnt;
}       dathdr = {0};
static struct {
    short int sx, sy, sz;
}       sdatpt = {0};

extern double sin();
extern double cos();
static char *fn = "bdat.dat";

main()
{
    int     i, n = 500, f, nb;
    double  a = 1000., dp, p, xc = 5, yc = 3;

    if ((f = bopen(fn)) < 0) {
	fprintf(stderr, "can't open file %s\n", fn);
	exit(1);
    }
    dathdr.magic = 12345;
    dathdr.nbpv = 2;
    dathdr.nvpp = 2;
    dathdr.npnt = n;
    if (write(f, &dathdr, sizeof(dathdr)) != sizeof(dathdr)) {
	fprintf(stderr, "Error writing file header\n");
	perror("gendat");
	exit(1);
    }
    dp = 6.2832 / (n - 1);
    nb = dathdr.nbpv * dathdr.nvpp;
    for (i = 0; i < n; i++) {
	p = i * dp;
	sdatpt.sx = a * cos(xc * p);
	sdatpt.sy = a * sin(yc * p);
	sdatpt.sz = 0;
	if (write(f, &sdatpt, nb) != nb) {
	    fprintf(stderr, "Error writing data\n");
	    perror("gendat");
	    exit(1);
	}
    }
    close(f);
    exit(0);
}
