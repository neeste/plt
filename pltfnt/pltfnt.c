/* pltfnt - reads and writes plt font definitions */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef MSDOS
#include <fcntl.h>		/* required for setmode */
#endif
#include "font.h"

#define DRAW 0
#define MOVE 1
#define CIRC 5
#define ARC  6
#define ENDC 7
#define FTYP 11
#define CHHW 12
#define FHAL 13
#define FVAL 14
#define GLYF 15
#define CMNT 16
#define FMAP 17
#define NAMESIZE 80
#define MAXN 8192
#define MAXK 256

double inqchh();
double qtxln();
int    chkdev();
int    get_line();
int    read_g();
int    read_m();
int    read_p();
int    read_t();
void   clsout();
void   opnout();
void   line();
void   marker();
void   newext();
void   text();
void   read_h();
void   setchh();
void   setmks();
void   setmkt();
void   setplt();
void   setplw();
void   setpps();
void   settxal();
void   settxft();
void   settxlw();
void   setwidestroke();
void   show_one();
void   show_all();
void   wrnodes();
void   usage();
void   wsep();
void   write_c();
void   write_g();
void   write_p();
void   write_t();
void   swab();
#ifdef linux
void   exit();
#endif

extern FONT *skfont;

static int newnod();

static short int ntmp[MAXN];
static short int ktmp[MAXK];
static short int map_a[MAXK];
static short int map_h[MAXK];
static short int dbug = 0;
static short int device = 1;
static short int header_size = 48;
static TRANSF unity = {1.0, 0.0, 0.0, 1.0, -127.0, -127.0};
static short int nmap = 0;
static char ifn[NAMESIZE] = "";
static char dfn[NAMESIZE] = "hersh1.dat";
static char ofn[NAMESIZE] = "";
static char ccode[20] = "dm...cae...fwhvg#>";
static char *iext[4] = {"", "txt", "pft", "mph"};
static char *oext[4] = {"", "txt", "pft", "c"};
static FONT *f_;

int
main(int ac, char **av)
{
    int     c, fnttyp, glyph, ifmt, ofmt;
    FILE   *ifp, *ofp;
    FONT   *fntalloc();

#if MSDOS
    device = chkdev();
#endif
    if (ac <= 1)
	usage();
    ifp = NULL;
    ofp = stdout;
    ifmt = ofmt = 0;
    fnttyp = 2;
    glyph = 0;
    while (ac > 1 && av[1][0] == '-') {
	switch (c = av[1][1]) {
	case 'c':
	case 'C':
	    ifmt = (c > 'a') ? 1 : 2;
	    ofmt = 3;
	    break;
	case 'd':
	    dbug++;
	    break;
	case 'f':
	    c = av[1][2];
	    if ('0' <= c && c <= '2')
		fnttyp = c - '0';
	    break;
	case 'h':
	    c = av[1][2];
	    if (c < '0' || c > '9') {
		ifmt = 3;
		ofmt = 1;
	    } else {
		ifmt = 4;
		ofmt = 6;
		glyph = atoi(av[1] + 2);
		ac = 1;
	    }
	    break;
	case 'm':
	    c = av[1][2];
	    if ('0' <= c && c <= '5')
		device = c - '0';
	    break;
	case 'p':
	    ifmt = 1;
	    ofmt = 2;
	    break;
	case 'r':
	case 'R':
	    ifmt = (c > 'a') ? 1 : 2;
	    ofmt = 0;
	    break;
	case 's':
	case 'S':
	    ifmt = (c > 'a') ? 1 : 2;
	    c = av[1][2];
	    if (c < '0' || c > '9')
		ofmt = 4;
	    else {
		ofmt = 5;
		glyph = atoi(av[1] + 2);
	    }
	    break;
	case 't':
	    ifmt = 2;
	    ofmt = 1;
	    break;
	}
	av++;
	ac--;
    }

    if (ac <= 1) {
	if (ifmt < 3)
	    ifmt = 0;
    }
    if (ifmt == 0)
	f_ = curfont[fnttyp];
    else
	f_ = fntalloc();
    do {
	if (ac > 1 && ifmt > 0 && ifmt < 4) {
	    strcpy(ifn, av[1]);
	    newext(ifn, iext[ifmt]);
	    if ((ifp = fopen(ifn, "r")) != NULL)
		(void) fprintf(stderr, "Input file: %s\n", ifn);
	    else {
		(void) fprintf(stderr, "can't open file: %s\n", ifn);
		exit(1);
	    }
	}
	if (ac > 1 && ofmt > 0 && ofmt < 4) {
	    strcpy(ofn, av[1]);
	    newext(ofn, oext[ofmt]);
	    if ((ofp = fopen(ofn, "w")) != NULL)
		(void) fprintf(stderr, "Output file: %s\n", ofn);
	    else {
		(void) fprintf(stderr, "can't open file: %s\n", ofn);
		exit(1);
	    }
	}
	switch (ifmt) {
	case 1:
	    fnttyp = read_t(ifp);
	    break;
	case 2:
	    fnttyp = read_p(ifp);
	    break;
	case 3:
	    fnttyp = read_m(ifp);
	    break;
	case 4:
	    fnttyp = read_g(glyph);
	    break;
	}
	switch (ofmt) {
	case 1:
	    write_t(ofp, fnttyp);
	    break;
	case 2:
	    write_p(ofp, fnttyp);
	    break;
	case 3:
	    write_c(ofp, fnttyp);
	    break;
	case 4:
	    show_all(fnttyp);
	    break;
	case 5:
	    show_one(fnttyp, glyph);
	    break;
	case 6:
	    write_g(glyph);
	    break;
	}
	if (ifp != NULL)
	    fclose(ifp);
	if (ofp != NULL && ofp != stdout)
	    fclose(ofp);
	ac--;
	av++;
    } while (ac > 1);

    return(0);
}

void
newext(char *fn, char *ex)
{
    while (*fn == '.')
	fn++;
    while (*fn != '\0' && *fn != '.')
	fn++;
    *fn++ = '.';
    while (*ex != '\0')
	*fn++ = *ex++;
    *fn++ = '\0';
}

void
write_c(FILE *ofp, int fnttyp)
{
    int     i, v, n;

    switch (fnttyp) {
    case 0:			/* standard font */
    case 1:			/* alternate font */
	wsep(ofp);

	(void) fprintf(ofp, "\n");
	(void) fprintf(ofp, "static char xlft[] = {");
	n = f_->nkhar;
	for (i = 0; i < n; i++) {
	    if ((i % 10) == 0)
		(void) fprintf(ofp, "\n    ");
	    v = f_->xlft[i];
	    (void) fprintf(ofp, "%5d%s", v, (i < n - 1) ? "," : "\n");
	}
	(void) fprintf(ofp, "};\n");

	(void) fprintf(ofp, "\n");
	(void) fprintf(ofp, "static char xadv[] = {");
	n = f_->nkhar;
	for (i = 0; i < n; i++) {
	    if ((i % 10) == 0)
		(void) fprintf(ofp, "\n    ");
	    v = f_->xadv[i];
	    (void) fprintf(ofp, "%5d%s", v, (i < n - 1) ? "," : "\n");
	}
	(void) fprintf(ofp, "};\n");

	(void) fprintf(ofp, "\n");
	(void) fprintf(ofp, "static short int khar[] = {");
	n = f_->nkhar + 1;
	for (i = 0; i < n; i++) {
	    if ((i % 10) == 0)
		(void) fprintf(ofp, "\n    ");
	    v = f_->khar[i];
	    (void) fprintf(ofp, "%5d%s", v, (i < n - 1) ? "," : "\n");
	}
	(void) fprintf(ofp, "};\n");

	(void) fprintf(ofp, "\n");
	(void) fprintf(ofp, "static short int node[] = {");
	n = f_->fnnd;
	for (i = 0; i < n; i++) {
	    if ((i % 8) == 0)
		(void) fprintf(ofp, "\n    ");
	    v = f_->node[i];
	    (void) fprintf(ofp, "%8d%s", v, (i < n - 1) ? "," : "\n");
	}
	(void) fprintf(ofp, "};\n");

	(void) fprintf(ofp, "\n");
	(void) fprintf(ofp, "FONT %s = {\n",
	    (fnttyp == 0) ? "stdfont_" : "altfont_");
	(void) fprintf(ofp, "%d,\n", f_->nkhar);
	(void) fprintf(ofp, "%d,\n", f_->fnnd);
	f_->ftyp = fnttyp;
	(void) fprintf(ofp, "%d, %d,\n", f_->ftyp, f_->fpsp);
	(void) fprintf(ofp, "%d, %d, %d, %d,\n",
	    f_->fchh, f_->fchw, f_->flnw, f_->fsln);
	(void) fprintf(ofp, "%d, %d,\n", f_->fnx0, f_->fny0);
	(void) fprintf(ofp, "{%d, %d, %d, %d},\n",
	    f_->fhal[0], f_->fhal[1], f_->fhal[2], f_->fhal[3]);
	(void) fprintf(ofp, "{%d, %d, %d, %d, %d, %d},\n",
	    f_->fval[0], f_->fval[1], f_->fval[2], f_->fval[3],
	    f_->fval[4], f_->fval[5]);
	(void) fprintf(ofp, "{0},\n");
	(void) fprintf(ofp, "xlft,\n");
	(void) fprintf(ofp, "xadv,\n");
	(void) fprintf(ofp, "khar,\n");
	(void) fprintf(ofp, "node\n");
	(void) fprintf(ofp, "};\n");

	(void) fprintf(ofp, "\n");
	wsep(ofp);
	break;

    case 2:			/* symbol font */
	wsep(ofp);

	(void) fprintf(ofp, "\n");
	(void) fprintf(ofp, "/* Symbol type stored in the 'styp' array\n");
	(void) fprintf(ofp, "/* 0=move&draw, 1=hollow, 2=filled, 3=fine-line\n");
	(void) fprintf(ofp, " */\n");
	(void) fprintf(ofp, "static char styp[] = {");
	n = f_->nkhar;
	for (i = 0; i < n; i++) {
	    if ((i % 10) == 0)
		(void) fprintf(ofp, "\n    ");
	    v = f_->xlft[i];
	    (void) fprintf(ofp, "%5d%s", v, (i < n - 1) ? "," : "\n");
	}
	(void) fprintf(ofp, "};\n");

	(void) fprintf(ofp, "\n");
	(void) fprintf(ofp, "/* The 'size factors' are stored in the 'sfac' array\n");
	(void) fprintf(ofp, " */\n");
	(void) fprintf(ofp, "static char sfac[] = {");
	n = f_->nkhar;
	for (i = 0; i < n; i++) {
	    if ((i % 10) == 0)
		(void) fprintf(ofp, "\n    ");
	    v = f_->xadv[i];
	    (void) fprintf(ofp, "%5d%s", v, (i < n - 1) ? "," : "\n");
	}
	(void) fprintf(ofp, "};\n");

	(void) fprintf(ofp, "\n");
	(void) fprintf(ofp, "static short int khar[] = {");
	n = f_->nkhar + 1;
	for (i = 0; i < n; i++) {
	    if ((i % 10) == 0)
		(void) fprintf(ofp, "\n    ");
	    v = f_->khar[i];
	    (void) fprintf(ofp, "%5d%s", v, (i < n - 1) ? "," : "\n");
	}
	(void) fprintf(ofp, "};\n");

	(void) fprintf(ofp, "\n");
	(void) fprintf(ofp, "static short int node[] = {");
	n = f_->fnnd;
	for (i = 0; i < n; i++) {
	    if ((i % 8) == 0)
		(void) fprintf(ofp, "\n    ");
	    v = f_->node[i];
	    (void) fprintf(ofp, "%8d%s", v, (i < n - 1) ? "," : "\n");
	}
	(void) fprintf(ofp, "};\n");

	(void) fprintf(ofp, "\n");
	(void) fprintf(ofp, "FONT symfont_ = {\n");
	(void) fprintf(ofp, "%d,\n", f_->nkhar);
	(void) fprintf(ofp, "%d,\n", f_->fnnd);
	(void) fprintf(ofp, "%d, %d,\n", f_->ftyp, f_->fpsp);
	(void) fprintf(ofp, "%d, %d, %d, %d,\n",
	    f_->fchh, f_->fchw, f_->flnw, f_->fsln);
	(void) fprintf(ofp, "%d, %d,\n", f_->fnx0, f_->fny0);
	(void) fprintf(ofp, "{%d, %d, %d, %d},\n",
	    f_->fhal[0], f_->fhal[1], f_->fhal[2], f_->fhal[3]);
	(void) fprintf(ofp, "{%d, %d, %d, %d, %d, %d},\n",
	    f_->fval[0], f_->fval[1], f_->fval[2], f_->fval[3],
	    f_->fval[4], f_->fval[5]);
	(void) fprintf(ofp, "{0},\n");
	(void) fprintf(ofp, "styp,\n");
	(void) fprintf(ofp, "sfac,\n");
	(void) fprintf(ofp, "khar,\n");
	(void) fprintf(ofp, "node\n");
	(void) fprintf(ofp, "};\n");

	(void) fprintf(ofp, "\n");
	wsep(ofp);
	break;

    }
}

void
write_t(FILE *ofp, int fnttyp)
{
    int     i, n;
    short int *fnode, *lnode;

    switch (fnttyp) {
    case 0:			/* standard font */
    case 1:			/* alternate font */
	(void) fprintf(ofp, "ftyp %5d %5d\n", f_->ftyp, f_->fpsp);
	(void) fprintf(ofp, "chhw %5d %5d %5d %5d\n",
	    f_->fchh, f_->fchw, f_->flnw, f_->fsln);
	(void) fprintf(ofp, "fhal %5d %5d %5d %5d\n",
	    f_->fhal[0], f_->fhal[1], f_->fhal[2], f_->fhal[3]);
	(void) fprintf(ofp, "fval %5d %5d %5d %5d %5d %5d\n",
	    f_->fval[0], f_->fval[1], f_->fval[2], f_->fval[3],
	    f_->fval[4], f_->fval[5]);
	(void) fprintf(ofp, "\n");
	break;

    case 2:			/* symbol font */
	(void) fprintf(ofp, "ftyp %5d %5d\n", f_->ftyp, f_->fpsp);
	(void) fprintf(ofp, "chhw %5d %5d %5d %5d\n",
	    f_->fchh, f_->fchw, f_->flnw, f_->fsln);
	(void) fprintf(ofp, "\n");
	break;

    }

    n = f_->nkhar;
    for (i = 0; i < n; i++) {
	fnode = f_->node + f_->khar[i] - 1;
	lnode = f_->node + f_->khar[i + 1] - 2;
	if (lnode >= fnode) {
	    (void) fprintf(ofp, "{ %5d %5d %5d\n", i, f_->xlft[i], f_->xadv[i]);
	    wrnodes(ofp, fnode, lnode);
	    (void) fprintf(ofp, "}\n");
	}
    }
}

void
write_g(int glyph)
{
    int     i;
    short int *fnode, *lnode;

    i = 0;
    fnode = f_->node + f_->khar[i] - 1;
    lnode = f_->node + f_->khar[i + 1] - 2;
    if (lnode >= fnode) {
	(void) fprintf(stdout, "{ %5d %5d %5d\n",
	    glyph, f_->xlft[i], f_->xadv[i]);
	wrnodes(stdout, fnode, lnode);
	(void) fprintf(stdout, "}\n");
    }
}

void
write_p(FILE *ofp, int fnttyp)
{
#ifdef MSDOS
    setmode(fileno(ofp), O_BINARY);	/* do binary output */
#endif
    f_->ftyp = fnttyp;
    fwrite(f_, header_size, 1, ofp);
    fwrite(f_->xlft, sizeof(*(f_->xlft)), f_->nkhar, ofp);
    fwrite(f_->xadv, sizeof(*(f_->xadv)), f_->nkhar, ofp);
    fwrite(f_->khar, sizeof(*(f_->khar)), f_->nkhar + 1, ofp);
    fwrite(f_->node, sizeof(*(f_->node)), f_->fnnd, ofp);
}

void
show_all(int fnttyp)
{
    char   *s, str[256];
    int     i;
    float   x, y, xx[1], yy[1];

    opnout(device);
    setwidestroke(1);
    settxlw(1.0);

    switch (fnttyp) {
    case 0:			/* standard font */
    case 1:			/* alternate font */
	setchh(0.2);
	settxal(2, 3);
	settxft(0);
	s = (fnttyp == 0) ? "STANDARD FONT" : "ALTERNATE FONT";
	sprintf(str, "%s: %s", s, ifn);
	text(5.25, 7.5, str);
	setchh(0.18);
	for (i = 0; i < 10; i++) {
	    x = 1.5;
	    y = 0.64 * (10 - (i % 10));
	    sprintf(str, "%d", i);
	    text(x, y, str);
	}
	for (i = 3; i < 13; i++) {
	    x = 0.75 * (i - 2) + 1.5;
	    y = 6.4 + 0.64;
	    sprintf(str, "%d", i * 10);
	    text(x, y, str);
	}
	curfont[fnttyp] = f_;
	settxft((fnttyp == 0) ? 0 : 11);
	for (i = 32; i < 127; i++) {
	    setmkt(i);
	    x = 0.75 * ((i / 10) - 2) + 1.5;
	    y = 0.64 * (10 - (i % 10));
	    sprintf(str, "%c", i);
	    text(x, y, str);
	}
	break;

    case 2:			/* symbol font */

	setchh(0.2);
	settxal(0, 0);
	text(0.5, 4.5, "SYMBOLS: ");
	text(0.5, 4.0, ifn);
	settxal(2, 3);
	for (i = 0; i < 10; i++) {
	    x = 4.0;
	    y = 0.7 * (10 - (i % 10));
	    sprintf(str, "%d", i);
	    text(x, y, str);
	}
	for (i = 0; i < 4; i++) {
	    x = 1.0 * (i + 1) + 4.0;
	    y = 7.5;
	    sprintf(str, "%d", i * 10);
	    text(x, y, str);
	}
	curfont[fnttyp] = f_;
	setmks(0.2);
	setplw((0.2 / 8) * 200);
	for (i = 0; i < 32; i++) {
	    setmkt(i);
	    xx[0] = 1.0 * (i / 10 + 1) + 4.0;
	    yy[0] = 0.7 * (10 - (i % 10));
	    marker(1, xx, yy);
	}
	break;

    }
    clsout();
}

void
show_one(int fnttyp, int glyph)
{
    char   *s, str[256];
    float   x, y, xx[2], yy[2];

    opnout(device);
    setwidestroke(1);
    settxlw(1.0);
    
    switch (fnttyp) {

    case 0:			/* standard font */
    case 1:			/* alternate font */
	setchh(0.2);
	settxal(2, 3);
	settxft(0);
	s = (fnttyp == 0) ? "STANDARD FONT" : "ALTERNATE FONT";
	sprintf(str, "%s: %s", s, ifn);
	text(5.25, 7.5, str);
	setchh(0.2);
	x = 1.5;
	y = 4.5;
	sprintf(str, "%d", glyph);
	text(x, y, str);
	curfont[fnttyp] = f_;
	settxft((fnttyp == 0) ? 0 : 11);
	setchh(4.0);
	settxal(2, 3);
	x = 5.0;
	y = 4.0;
	sprintf(str, "%c", glyph);
	text(x, y, str);

	setpps(inqchh() / f_->fchh);
	setplt(1);
	yy[0] = y - 0.5 * inqchh();
	yy[1] = y + 0.5 * inqchh();
	xx[0] = xx[1] = x - 0.5 * qtxln(str);
	line(2, xx, yy);
	xx[0] = xx[1] = x + 0.5 * qtxln(str);
	line(2, xx, yy);
	xx[0] = x - 0.5 * qtxln(str);
	xx[1] = x + 0.5 * qtxln(str);
	yy[1] = yy[0] = y - 0.5 * inqchh();
	line(2, xx, yy);
	xx[0] = x - 0.5 * qtxln(str);
	xx[1] = x + 0.5 * qtxln(str);
	yy[1] = yy[0] = y + 0.5 * inqchh();
	line(2, xx, yy);
	xx[0] = x - 0.5 * qtxln(str);
	xx[1] = x + 0.5 * qtxln(str);
	yy[1] = yy[0] = y + f_->fval[5] * inqchh() / f_->fchh;
	line(2, xx, yy);

	setchh(0.2);
	sprintf(str, "f_->fval: %d %d %d %d %d %d",
	    f_->fval[0], f_->fval[1], f_->fval[2], f_->fval[3],
	    f_->fval[4], f_->fval[5]);
	text(5.0, 0.5, str);

	break;

    case 2:			/* symbol font */
	setchh(0.2);
	settxal(2, 3);
	sprintf(str, "%s: %s", "SYMBOLS", ifn);
	text(5.25, 7.5, str);
	x = 1.5;
	y = 4.5;
	sprintf(str, "%d", glyph);
	text(x, y, str);
	curfont[fnttyp] = f_;
	setmks(3.0);
	setplw((3.0 / 8) * 200);
	setmkt(glyph);
	xx[0] = 5.0;
	yy[0] = 4.0;
	marker(1, xx, yy);
	break;
    }
    clsout();
}

void
wsep(ofp)
FILE   *ofp;
{
    char   *s = "=====================================";

    (void) fprintf(ofp, "/*%s%s*/\n", s, s);
}

void
wrnodes(ofp, fnode, lnode)
FILE   *ofp;
short int *fnode, *lnode;
{
    short int *pnode;
    int     nodtyp, deg;
    float   x, y;
    TRANSF *n = &unity;

    for (pnode = fnode; pnode <= lnode; pnode++) {
	nodtyp = newnod(pnode, &x, &y, n);
	switch (nodtyp) {
	case DRAW:
	    if (pnode > fnode)
		(void) fprintf(ofp, "d %5d %5d\n", (int) x, (int) y);
	    else
		(void) fprintf(ofp, "m %5d %5d\n", (int) x, (int) y);
	    break;
	case MOVE:
	    nodtyp = newnod(++pnode, &x, &y, n);
	    (void) fprintf(ofp, "m %5d %5d\n", (int) x, (int) y);
	    break;
	case CIRC:
	case ARC:
	    deg = (nodtyp == CIRC) ? 360 : *++pnode;
	    nodtyp = newnod(++pnode, &x, &y, n);
	    nodtyp = (deg / 360 == 0) ? ARC : CIRC;
	    if (nodtyp == CIRC)
		(void) fprintf(ofp, "c %5d %5d\n", (int) x, (int) y);
	    else
		(void) fprintf(ofp, "a %5d %5d %5d\n", (int) x, (int) y, (int) deg);
	    break;
	case ENDC:
	    return;
	}
    }
}

/*
 * newnod - unpack coordinate bytes (nx,ny) from node
 */
static int
newnod(node, px, py, n)
short int *node;
float  *px, *py;
TRANSF *n;
{
    int     nx, ny;
    char   *nd;

    nd = (char *) node;
    ny = nd[0] & 255;
    nx = nd[1] & 255;
    if (nx == 255)
	return (ny);
    *px = n->txx * nx + n->txy * ny + n->txo;
    *py = n->tyx * nx + n->tyy * ny + n->tyo;
    return (0);
}

int
read_t(FILE *ifp)
{
    int     i, j, fnttyp, nch, nnd, cmd, n;
    int     gsz, maxgsz, filsym, lft, adv;
    short int arg[8];

    for (i = 0; i < MAXK; i++)
	ktmp[i] = 0;
    ntmp[0] = ENDC - 256;
    nnd = 1;
    maxgsz = 0;
    fnttyp = 2;
    while ((cmd = get_line(ifp, arg, 6)) != EOF) {
	switch (cmd) {
	case FTYP:
	    f_->ftyp = arg[0];
	    f_->fpsp = arg[1];
	    fnttyp = f_->ftyp;
	    break;
	case CHHW:
	    f_->fchh = arg[0];
	    f_->fchw = arg[1];
	    f_->flnw = arg[2];
	    f_->fsln = arg[3];
	    if (fnttyp != 2) {	/* set default lft and adv */
		lft = 0;
		adv = f_->fchw;
		for (i = 0; i < MAXK; i++) {
		    f_->xlft[i] = lft;
		    f_->xadv[i] = adv;
		}
	    }
	    break;
	case FHAL:
	    for (i = 0; i < 4; i++)
		f_->fhal[i] = arg[i];
	    break;
	case FVAL:
	    for (i = 0; i < 6; i++)
		f_->fval[i] = arg[i];
	    break;
	case GLYF:
	    i = arg[0];
	    if (i < 0 || i > MAXK)
		break;
	    f_->xlft[i] = arg[1];
	    f_->xadv[i] = arg[2];
	    filsym = (f_->ftyp == 2 && arg[1] > 0 && arg[1] < 3);
	    ktmp[i] = nnd;
	    gsz = 0;
	    do {
		cmd = get_line(ifp, arg, 3);
		n = ((arg[0] + 127) << 8) | (arg[1] + 127);
		switch (cmd) {
		case MOVE:
		    if (nnd > ktmp[i])
			ntmp[nnd++] = MOVE - 256;
		    ntmp[nnd++] = n;
		    gsz++;
		    break;
		case DRAW:
		    ntmp[nnd++] = n;
		    gsz++;
		    break;
		case ARC:
		    ntmp[nnd++] = ARC - 256;
		    ntmp[nnd++] = arg[2];
		    ntmp[nnd++] = n;
		    gsz += (64 * arg[2]) / 360;
		    break;
		case CIRC:
		    ntmp[nnd++] = CIRC - 256;
		    ntmp[nnd++] = n;
		    gsz += 64;
		    break;
		case ENDC:
		    ntmp[nnd++] = ENDC - 256;
		    break;
		}
		if (nnd >= MAXN) {
		    (void) fprintf(stderr, "ERROR: too many nodes\n");
		    break;
		}
	    } while (cmd != ENDC && cmd != EOF);
	    if (gsz > maxgsz && filsym)
		maxgsz = gsz;
	    break;
	}
    }
    if (maxgsz > 0)
	(void) fprintf(stderr, "maximum glyph size = %d\n", maxgsz);
    nnd = 0;
    nch = 0;
    for (i = 0; i < MAXK; i++) {
	f_->khar[i] = nnd + 1;
	for (j = ktmp[i]; j < MAXN; j++) {
	    if (ntmp[j] == (ENDC - 256))
		break;
	    f_->node[nnd++] = ntmp[j];
	    nch = i + 1;
	    if (nnd >= MAXN) {
		break;
	    }
	}
    }
    f_->khar[MAXK] = nnd + 1;
    f_->fnnd = nnd;
    f_->nkhar = nch;
    (void) fprintf(stderr, "nkhar=%d, fnnd=%d\n", f_->nkhar, f_->fnnd);
    if (nnd >= MAXN)
	(void) fprintf(stderr, "ERROR: too many nodes in .txt file\n");
    return (fnttyp);
}

int
read_m(ifp)
FILE   *ifp;
{
    int     i, j, fnttyp, nnd, nch, cmd;
    short int arg[8];

    nmap = 0;
    fnttyp = 2;
    while ((cmd = get_line(ifp, arg, 6)) != EOF) {
	switch (cmd) {
	case FTYP:
	    f_->ftyp = arg[0];
	    f_->fpsp = arg[1];
	    fnttyp = f_->ftyp;
	    break;
	case CHHW:
	    f_->fchh = arg[0];
	    f_->fchw = arg[1];
	    f_->flnw = arg[2];
	    f_->fsln = arg[3];
	    break;
	case FHAL:
	    for (i = 0; i < 4; i++)
		f_->fhal[i] = arg[i];
	    break;
	case FVAL:
	    for (i = 0; i < 6; i++)
		f_->fval[i] = arg[i];
	    break;
	case FMAP:
	    map_a[nmap] = arg[0];
	    map_h[nmap] = arg[1];
	    nmap++;
	    break;
	}
    }

    read_h();

    nnd = 0;
    nch = 0;
    for (i = 0; i < MAXK; i++) {
	f_->khar[i] = nnd + 1;
	for (j = ktmp[i]; j < MAXN && nnd < MAXN; j++) {
	    if (ntmp[j] == (ENDC - 256))
		break;
	    f_->node[nnd++] = ntmp[j];
	    nch = i + 1;
	}
    }
    f_->khar[MAXK] = nnd + 1;
    f_->nkhar = nch;
    f_->fnnd = nnd;
    (void) fprintf(stderr, "nkhar=%d, fnnd=%d\n", f_->nkhar, f_->fnnd);
    if (nnd >= MAXN)
	(void) fprintf(stderr, "ERROR: too many nodes in file, > %d\n", nnd);
    return (fnttyp);
}

int
read_g(int glyph)
{
    int     i, j, fnttyp, nnd, nch;

    f_->ftyp = 0;
    f_->fpsp = 1;
    fnttyp = f_->ftyp;
    map_a[0] = 0;
    map_h[0] = glyph;
    nmap = 1;

    read_h();

    nnd = 0;
    nch = 0;
    for (i = 0; i < MAXK; i++) {
	f_->khar[i] = nnd + 1;
	for (j = ktmp[i]; j < MAXN && nnd < MAXN; j++) {
	    if (ntmp[j] == (ENDC - 256))
		break;
	    f_->node[nnd++] = ntmp[j];
	    nch = i + 1;
	}
    }
    f_->khar[MAXK] = nnd + 1;
    f_->nkhar = nch;
    f_->fnnd = nnd;
    (void) fprintf(stderr, "nkhar=%d, fnnd=%d\n", f_->nkhar, f_->fnnd);
    if (nnd >= MAXN)
	(void) fprintf(stderr, "ERROR: too many nodes in file > %d\n", nnd);
    return (fnttyp);
}

int
read_p(FILE *ifp)
{
    char    buf[16];
    int     fnttyp, swpbyt = 0, nch, nnd, nr;

#ifdef MSDOS
    setmode(fileno(ifp), O_BINARY);	/* do binary input */
#endif
    nr = fread(f_, 32, 1, ifp);
    nr = fread(buf, header_size - 32, 1, ifp);
    if (f_->nkhar <= 0 || f_->nkhar > MAXNCHR) {
	swab((char *) &f_->nkhar, (char *) &f_->nkhar, sizeof(short) * 2);
	if (f_->nkhar <= 0 || f_->nkhar > MAXNCHR) {
	    (void) fprintf(stderr, "ERROR: bad header in .pft font file\n");
	    exit(1);
	}
	swpbyt = 1;
	fgetc(stderr);
    }
    nch = f_->nkhar;
    nnd = f_->fnnd;
    fnttyp = f_->ftyp;
    if (fnttyp < 0 || fnttyp > 2) {
	(void) fprintf(stderr, "ERROR: bad header in .pft font file\n");
	exit(1);
    }
    (void) fprintf(stderr, "nkhar=%d, fnnd=%d\n", nch, nnd);
    if (nch > MAXK || nnd > MAXN) {
	(void) fprintf(stderr, "ERROR: not enough room for font data\n");
	exit(1);
    }
    nr = fread(f_->xlft, sizeof(char), nch, ifp);
    nr = fread(f_->xadv, sizeof(char), nch, ifp);
    nr = fread(f_->khar, sizeof(short), nch + 1, ifp);
    nr = fread(f_->node, sizeof(short), nnd, ifp);
    if (swpbyt) {
	swab((char *) f_->khar, (char *) f_->khar, sizeof(short) * (nch + 1));
	swab((char *) f_->node, (char *) f_->node, sizeof(short) * nnd);
    }
    nr = nr; /* use nr to avoid error */

    return (fnttyp);
}

int
get_line(FILE *ifp, short *arg, int n)
{
    char   *s, line[80];
    int     i, cmd;

    if (fgets(line, 80, ifp) == NULL)
	return (EOF);
    else if (strncmp(line, "ftyp", 4) == 0)
	cmd = FTYP;
    else if (strncmp(line, "chhw", 4) == 0)
	cmd = CHHW;
    else if (strncmp(line, "fhal", 4) == 0)
	cmd = FHAL;
    else if (strncmp(line, "fval", 4) == 0)
	cmd = FVAL;
    else if (isdigit(line[0]))
	cmd = FMAP;
    else if (line[0] == '{')
	cmd = GLYF;
    else if (line[0] == 'm')
	cmd = MOVE;
    else if (line[0] == 'd')
	cmd = DRAW;
    else if (line[0] == 'a')
	cmd = ARC;
    else if (line[0] == 'c')
	cmd = CIRC;
    else if (line[0] == '}')
	cmd = ENDC;
    else
	cmd = CMNT;
    s = line;
    if (dbug)
	(void) fprintf(stderr, "%s", s);
    while (!isspace(*s) && !isdigit(*s) && *s != '\0')	/* skip 1st arg */
	s++;
    for (i = 0; i < n; i++) {
	while (isspace(*s))
	    s++;
	arg[i] = atoi(s);
	while (!isspace(*s) && *s != '\0')
	    s++;
    }
    if (dbug) {
	(void) fprintf(stderr, "cmd=%d, args=", cmd);
	for (i = 0; i < n; i++)
	    (void) fprintf(stderr, " %d", arg[i]);
	(void) fprintf(stderr, "\n");
    }
    return (cmd);
}

void
read_h()
{
    char    line[256], *s;
    int     i, j, htmp, atmp, glyph, x, y, cmd, nnd, n, k;
    FILE   *dfp;

    if ((dfp = fopen(dfn, "r")) != NULL)
	(void) fprintf(stderr, "Data file: %s\n", dfn);
    else {
	(void) fprintf(stderr, "can't open file: %s\n", dfn);
	exit(1);
    }
    if (dbug)
	fprintf(stderr, "------------------------------------------------\n");

/* sort the map list */
    for (i = 0; i < nmap; i++)
	for (j = i + 1; j < nmap; j++)
	    if (map_h[j] < map_h[i]) {
		htmp = map_h[j];
		atmp = map_a[j];
		map_h[j] = map_h[i];
		map_a[j] = map_a[i];
		map_h[i] = htmp;
		map_a[i] = atmp;
	    }
/* initialize all glyphs to null */
    for (i = 0; i < MAXK; i++)
	ktmp[i] = 0;
    ntmp[0] = ENDC - 256;

/* find the needed glyphs */
    nnd = 1;
    if (fgets(line, 255, dfp) == NULL) {
	fprintf(stderr, "ERROR: no data in hershey data file\n");
	return;
    }
    glyph = atoi(line);
    for (i = 0; i < nmap; i++) {
	if (map_a[i] < 0 || map_a[i] >= 127) {
	    fprintf(stderr, "ERROR: character out of range = %d\n", map_a[i]);
	    continue;
	}
	if (map_h[i] <= 0 || map_h[i] >= 4000) {
	    fprintf(stderr, "ERROR: glyph out of range = %d\n", map_h[i]);
	    continue;
	}
	if (map_h[i] == map_h[i - 1]) {
	    ktmp[map_a[i]] = ktmp[map_a[i - 1]];
	    continue;
	}
	while (glyph != map_h[i]) {
	    if (fgets(line, 255, dfp) == NULL)
		break;
	    glyph = atoi(line);
	}
	if (glyph != map_h[i]) {
	    fprintf(stderr, "ERROR: glyph not found = %d\n", map_h[i]);
	    break;
	}
	if (dbug)
	    fprintf(stderr, "ascii %d, hershey %d:\n", map_a[i], map_h[i]);
	k = map_a[i];
	cmd = GLYF;
	do {
	    for (j = 0; j < 15 && cmd != ENDC; j++) {
		s = line + 7 + j * 8;
		if (*s != ':')
		    break;
		x = atoi(s + 1);
		y = atoi(s + 4);
		if (cmd != GLYF && x == -64) {
		    if (y == -64) {
			cmd = ENDC;
		    } else if (y == 0) {
			cmd = MOVE;
		    }
		} else {
		    if (dbug)
			fprintf(stderr, "    %c %4d %4d\n", ccode[cmd], x, y);
		    if (cmd == GLYF) {
			f_->xlft[k] = -x;
			f_->xadv[k] = y - x;
			ktmp[k] = nnd;
			cmd = MOVE;
		    } else {
			n = ((x + 127) << 8) | (-y + 127);
			if (cmd == MOVE && nnd > ktmp[k])
			    ntmp[nnd++] = MOVE - 256;
			ntmp[nnd++] = n;
			cmd = DRAW;
		    }
		    if (nnd >= MAXN) {
			(void) fprintf(stderr, "ERROR: too many nodes\n");
			return;
		    }
		}
	    }
	    if (fgets(line, 255, dfp) == NULL)
		break;
	    glyph = atoi(line);
	} while (glyph == 0);
	if (dbug)
	    fprintf(stderr, ".\n");
	ntmp[nnd++] = ENDC - 256;
    }
}

FONT   *
fntalloc()
{
    FONT   *t_;

    t_ = (FONT *) calloc(1, sizeof(FONT));
    if (t_ == NULL) {
	(void) fprintf(stderr, "ERROR: not enough memory for struct\n");
	exit(1);
    }
    t_->fnx0 = 127;
    t_->fny0 = 127;
    t_->xlft = (char *) calloc(MAXK, sizeof(char));
    t_->xadv = (char *) calloc(MAXK, sizeof(char));
    t_->khar = (short int *) calloc(MAXK + 1, sizeof(short int));
    t_->node = (short int *) calloc(MAXN, sizeof(short int));
    if (t_->node == NULL) {
	(void) fprintf(stderr, "ERROR: not enough memory for arrays\n");
	exit(1);
    }
    return (t_);
}

void
usage()
{
    (void) fprintf(stderr, "\n");
    (void) fprintf(stderr, "usage: pltfnt [options] [filename] ...\n");
    (void) fprintf(stderr, "options:\n");
    (void) fprintf(stderr, "    -c    output C code from .txt file\n");
    (void) fprintf(stderr, "    -C    output C code from .pft file\n");
    (void) fprintf(stderr, "    -d    enable debug print\n");
    (void) fprintf(stderr, "    -fN   use internal fonts, N = 0, 1, 2\n");
    (void) fprintf(stderr, "          for standard, alternate, or symbol\n");
    (void) fprintf(stderr, "    -hN   get glyphs from Hershey data base\n");
    (void) fprintf(stderr, "          N = Hershey glyph number\n");
    (void) fprintf(stderr, "    -mN   output mode=N: 0=tekplot, 1=tekterm,\n");
    (void) fprintf(stderr, "          2=impress, 3=hpgl, 4=console, 5=postscript\n");
    (void) fprintf(stderr, "    -p    output .pft file from .txt file\n");
    (void) fprintf(stderr, "    -r    read (only) from .txt file\n");
    (void) fprintf(stderr, "    -R    read (only) from .pft file\n");
    (void) fprintf(stderr, "    -sN   output screen from .txt file\n");
    (void) fprintf(stderr, "    -SN   output screen from .pft file\n");
    (void) fprintf(stderr, "    -t    output .txt file from .pft file\n");
    (void) fprintf(stderr, "\n");
    exit(0);
}

