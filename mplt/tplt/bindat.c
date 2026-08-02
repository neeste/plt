/*
 * bindat - open include file
 *
 *	Functions called:
 *		    _open, _close, errout, indx, _read, sprintf, strcat, _tell
 *
 *	Called by:
 *		    pltkey
 *
 */
#include <string.h>
#include <fcntl.h>
#include "plt.h"
#include "pltblk.h"
#include "cincl.h"

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#include <sys/file.h>
#define _tell(fd)       lseek(fd,0,1)
#define _open           open
#define _close          close
#define _read           read
#define _swab           swab
#endif
#ifdef linux
void swab();
#endif
#ifndef O_BINARY
#define O_BINARY 0
#endif
#define bopen(fn)	_open(fn, O_RDONLY | O_BINARY)

#define LSB  ((char *) &byt_ord)[0]
#define MSB  ((char *) &byt_ord)[1]

int     indx(char *, int);
int     mordat(void);
void    getfn(char *, char *, int, char *);
void    getpn(char *, char *);
void    tpl(double, double, double, double);

static int bin_read(int, char *, int);

extern int maxnp;
extern int ndatp;
extern float *xdat, *ydat, *zdat;

static struct {short int magic, nbpv, nvpp, npnt;} dathdr = {0};
static struct {short int sx, sy, sz;} sdatpt = {0};
static struct {long int lx, ly, lz;} ldatpt = {0};
static short byt_ord = 256;

void
bindat(char *instr)
{
    char    fn[NAMESIZE];
    char    pn[NAMESIZE];
    int     i, n, nb, nbr;
    double  col1, col2, col3;
    float  *px, *py, *pz;
    int     fd;

    getfn(instr, fn, NAMESIZE, ".dat");
    getpn(pn, fn);
#ifdef DEBUG
    if (flags_.dbflg) {
	sprintf(ctext_.msg, " bindat: data file %s\n", pn);
	errout(ctext_.msg);
    }
#endif

    if ((fd = bopen(pn)) < 0) {
	sprintf(ctext_.msg, " *** can't open BDATA file: %s\n", pn);
	errout(ctext_.msg);
	return;
    }
    nbr = bin_read(fd, (char *) &dathdr.magic, (int) sizeof(dathdr));
    if (nbr != sizeof(dathdr) || dathdr.magic != 12345
	|| (dathdr.nbpv != 2 && dathdr.nbpv != 4)
	|| dathdr.nvpp < 1 || dathdr.nvpp > 3) {
	sprintf(ctext_.msg, " *** BAD header in BDATA file: %s\n", pn);
	errout(ctext_.msg);
	sprintf(ctext_.msg, " magic = %d, nbpv = %d, nvpp=%d, npnt=%d\n",
	    dathdr.magic, dathdr.nbpv, dathdr.nvpp, dathdr.npnt);
	errout(ctext_.msg);
	return;
    }
    n = dathdr.npnt;
    while ((ndatp + n) > maxnp)
	if (mordat()== 0)
	    break;
    if ((ndatp + n) > maxnp)
	n = maxnp - ndatp;
    px = xdat + ndatp;
    py = ydat + ndatp;
    pz = zdat + ndatp;
    sdatpt.sx = sdatpt.sy = sdatpt.sz = 0;
    ldatpt.lx = ldatpt.ly = ldatpt.lz = 0;
    nb = dathdr.nbpv * dathdr.nvpp;
    for (i = 0; i < n; i++) {
	if (dathdr.nbpv == 2) {
	    nbr = bin_read(fd, (char *) &sdatpt.sx, nb);
	    if (nbr != nb) {
		errout("WARNING: error reading binary data\n");
		sprintf(ctext_.msg,
		    "at offset %ld in file %s\n", (long) _tell(fd), pn);
		errout(ctext_.msg);
		break;
	    }
	    col1 = sdatpt.sx;
	    col2 = sdatpt.sy;
	    col3 = sdatpt.sz;
	} else {
	    nbr = bin_read(fd, (char *) &ldatpt.lx, nb);
	    if (nbr != nb) {
		errout("WARNING: error reading binary data\n");
		sprintf(ctext_.msg,
		    "at offset %ld in file %s.\n", (long) _tell(fd), pn);
		errout(ctext_.msg);
		break;
	    }
	    col1 = ldatpt.lx;
	    col2 = ldatpt.ly;
	    col3 = ldatpt.lz;
	}
	*px++ = (float) (col1 * param_.xgain + param_.xofst);
	*py++ = (float) (col2 * param_.ygain + param_.yofst);
	*pz++ = (float) (col3 * param_.zgain + param_.zofst);
    }
    ndatp += i;
    _close(fd);
}

static int
bin_read(int d, char *b, int n)
{
    int     s;

    s = (int)_read(d, b, n);
    if (MSB == 0)
        _swab(b, b, n);
    return (s);
}
