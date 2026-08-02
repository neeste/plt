/* pltblk.h - Global variables for PLT */

struct param {
    float   anncol, annlwt, annsiz, aronum, arosiz, arotip, arowid, 
	    axcol, axlwt, bgcol, cdext, clip, corner, count, 
	    ebnum, ebwid, echo, fgcol, filcol, fncol, gapsiz, grdcol, 
	    grdlwt, grdsiz, grdtyp, grid, header, labcol, lablwt, 
	    labsiz, like, lindot, linsiz, lintyp, magnify, mhalign, mhkey,
            mhsp, mhxp, msgang, msgcol, msgdat, msglwt, msgsiz, mslant, 
	    mvalign, mvkey, mvsp, mxdat, mxllc, mydat, myllc, ndata, 
	    norm, openax, pltcol, pltlwt, pltype, ranskp, raxis, rbreak, 
	    rcycle, rint, rmax, rmin, rotate, rper, rshift, rtick, shade, 
	    shdcol, shdgray, shdlwt, sizfac, smooth, solid, sort, spline, 
	    stem, symang, symbol, symsiz, tanskp, taxis, tbreak, tcycle, 
	    ticdir, ticsiz, tint, tmax, tmin, tper, tshift, ttick, vxllc, 
	    vyllc, wxlen, wxllc, wylen, wyllc, xanskp, xaxis, xbreak, 
	    xcycle, xgain, xint, xlen, xllc, xmax, xmin, xofst, xper, 
	    xshift, xtick, yanskp, yaxis, ybreak, ycycle, ygain, yhor, yint, 
	    ylen, yllc, ymax, ymin, yofst, yper, yshift, ytick, zgain, zofst; 
};
extern struct param param_;

struct flags {
    char    axflg, newpag, dbflg, finish, rngflg, exit;
};
extern struct flags flags_;

struct ctext {
    char    msg[MAXMSG*2];
    char   *xfmt, *yfmt, *tfmt, *rfmt;
    char   *xlabel, *ylabel, *tlabel, *rlabel;
    char   *xannot, *yannot, *tannot, *rannot;
    char   *xdata, *ydata, *zdata;
    char   *select;
};
extern struct ctext ctext_;

struct misc {
    char    hdate[26];
    char    filinm[NAMESIZE], filonm[NAMESIZE], curpltdir[NAMESIZE];
    short   ifrom, ito, inow, ipage, device;
    short   pltyet, hdryet, slomod, in_tt;
    short   numpen, pgprmt, pgbord, colors, lftovr, npkept;
    short   i_pltype, i_ticdir, i_mhalign, i_mvalign;
    FILE   *lunin, *lunout;
};
extern struct misc misc_;

struct axes {
    short   logx, logy, logt, logr;
    short   xaxis, yaxis, raxis, taxis;
    float   axx, ayy, att, arr;
    float   axo, ayo, ato, aro;
    float   rxmin, rxmax, rxlen, rxllc;
    float   rymin, rymax, rylen, ryllc;
};
extern struct axes axes_;

struct size {
    float   avgaxl, tiklen, hitann, hitlab, hitsym, hitmsg;
    float   annlnw, lablnw, pltlnw, msglnw, shdlnw, axlnw, grdlnw;
    float   pltlps, grdlps, arosiz;
};
extern struct size size_;

