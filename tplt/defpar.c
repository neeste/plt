/*
 * defpar - set parameters to default values
 */
#include "plt.h"
#include "pltblk.h"
#include "cincl.h"
#include "fort.h"

#define CNULL '\0'

extern double inch_unit;

void
defpar(void)
{
/*
 *     X axis parameters
 */
    param_.xaxis = 1;			/* xaxis */
    param_.xmin = 0;			/* xmin */
    param_.xmax = 10;			/* xmax */
    param_.xint = 5;			/* xint */
    param_.xcycle = 0;			/* xcycle */
    param_.xper = 100;			/* xpercent */
    param_.xshift = 0;			/* xshift */
    param_.xbreak = 0;			/* xbreak */
    param_.xlen = (float) (6.0 * inch_unit);	/* xlen */
    param_.xllc = (float) (2.0 * inch_unit);	/* xllc */
    param_.xtick = 1;			/* xtick */
/*
 *     Y axis parameters
 */
    param_.yaxis = 1;			/* yaxis */
    param_.ymin = 0;			/* ymin */
    param_.ymax = 10;			/* ymax */
    param_.yint = 5;			/* yint */
    param_.ycycle = 0;			/* ycycle */
    param_.yper = 100;			/* ypercent * */
    param_.yshift = 0;			/* yshift */
    param_.ybreak = 0;			/* ybreak */
    param_.ylen = (float) (6.0 * inch_unit);	/* ylen */
    param_.yllc = (float) (1.5 * inch_unit);	/* yllc */
    param_.ytick = 1;			/* ytick */
/*
 *     Top axis parameters
 */
    param_.taxis = 1;			/* taxis */
    param_.tmin = 0;			/* tmin */
    param_.tmax = 0;			/* tmax */
    param_.tint = 5;			/* tint */
    param_.tcycle = 0;			/* tcycle */
    param_.tper = 100;			/* tpercent * */
    param_.tshift = 0;			/* tshift */
    param_.tbreak = 0;			/* tbreak */
    param_.ttick = 1;			/* ttick */
/*
 *     Right-hand axis parameters
 */
    param_.raxis = 1.0;		/* raxis */
    param_.rmin = 0.0;		/* rmin */
    param_.rmax = 0.0;		/* rmax */
    param_.rint = 5.0;		/* rint */
    param_.rcycle = 0.0;	/* rcycle */
    param_.rper = 100.0;	/* rpercent * */
    param_.rshift = 0.0;	/* rshift */
    param_.rbreak = 0.0;	/* rbreak */
    param_.rtick = 1.0;		/* rtick */
/*
 *     Other axis parameters
 */
    param_.axcol = -1.0;	/* axcol */
    param_.axlwt = 1.0;		/* axlwt */
    param_.openax = 0.0;	/* openax */
    param_.ticdir = 0.0;	/* ticdir */
    param_.ticsiz = 1.0;	/* ticsiz */
    param_.corner = 1.0;	/* corner */
/*
 *     Parameters for annotations and labels
 */
    param_.annsiz = 1.0;	/* annsiz */
    param_.anncol = -1.0;	/* anncol */
    param_.annlwt = 1.0;	/* annlwt */
    param_.xanskp = 0.0;	/* xanskp */
    param_.yanskp = 0.0;	/* yanskp */
    param_.tanskp = 0.0;	/* tanskp */
    param_.ranskp = 0.0;	/* ranskp */
    param_.yhor = 0.0;		/* yhor */
    param_.labsiz = 1.0;	/* labsiz */
    param_.labcol = -1.0;	/* labcol */
    param_.lablwt = 1.0;	/* lablwt */
/*
 *     Parameters for grid lines
 */
    param_.grid = 0.0;		/* grid */
    param_.grdcol = -1.0;	/* grdcol */
    param_.grdlwt = 1.0;	/* grdlwt */
    param_.grdsiz = 1.0;	/* grdsiz */
    param_.grdtyp = 1.0;	/* grdtyp */
/*
 *     Parameters for plotted data
 */
    param_.pltype = 3.0;		    /* pltype */
    param_.linsiz = 1.0;		    /* linsiz */
    param_.lintyp = 0.0;		    /* lintyp */
    param_.lindot = 0.0;		    /* lindot */
    param_.pltcol = -1.0;		    /* pltcol */
    param_.pltlwt = 1.0;		    /* pltlwt */
    param_.symbol = 1.0;		    /* symbol */
    param_.symsiz = 1.0;		    /* symsiz */
    param_.gapsiz = 1.0;		    /* gapsiz */
    param_.solid = 0.0;			    /* solid */
    param_.symang = 0.0;		    /* symang */
    param_.filcol = -1.0;		    /* filcol */
/*
 *     Input parameters
 */
    param_.xgain = 1.0;			    /* xgain */
    param_.ygain = 1.0;			    /* ygain */
    param_.zgain = 1.0;			    /* zgain */
    param_.xofst = 0.0;			    /* xofst */
    param_.yofst = 0.0;			    /* yofst */
    param_.zofst = 0.0;			    /* zofst */
    param_.ndata = 1.0;			    /* ndata */
/*
 *     Output parameters
 */
    param_.wxllc = 0;			    /* wxllc */
    param_.wyllc = 0;			    /* wyllc */
    param_.wxlen = (float) (XPAGE * inch_unit);  /* wxlen */
    param_.wylen = (float) (YPAGE * inch_unit);   /* wylen */
    param_.vxllc = 0;			    /* vxllc */
    param_.vyllc = 0;			    /* vyllc */
    param_.magnify = 1;			    /* magnify */
    param_.rotate = 0;			    /* rotate */
/*
 *     Parameters for messages
 */
    param_.msgsiz = 1.0;		    /* msgsiz * */
    param_.msgcol = -1.0;		    /* msgcol * */
    param_.msglwt = 1.0;		    /* msglwt * */
    param_.mxllc = 0.0;			    /* mxllc * */
    param_.myllc = 0.0;			    /* myllc * */
    param_.msgang = 0.0;		    /* msgangl * */
    param_.msgdat = 0.0;		    /* msgdat * */
    param_.mxdat = 0.0;			    /* mxdat * */
    param_.mydat = 0.0;			    /* mydat * */
    param_.mhalign = 0.0;		    /* mhalign * */
    param_.mvalign = 0.0;		    /* mvalign * */
    param_.mslant = 0.0;		    /* mslant * */
    param_.mhkey = 3.0;			    /* mhkey */
    param_.mvkey = 1.0;			    /* mvkey */
    param_.mhxp = 1.0;			    /* mhxp */
    param_.mhsp = 0.0;			    /* mhsp */
    param_.mvsp = (float) 1.64;		    /* mvsp */
/*
 *     Shade parameters
 */
    param_.shade = 0.0;		/* shade */
    param_.shdcol = -1.0;	/* shdcol */
    param_.shdgray = 1.0;	/* shdgray */
    param_.shdlwt = 1.0;	/* shdlwt */
/*
 *     Arrow parameters
 */
    param_.aronum = (float) 0;
    param_.arosiz = (float) 1;
    param_.arotip = (float) 0.1;
    param_.arowid = (float) 0.5;
/*
 *     Other parameters
 */
    param_.echo = 0.0;		/* echo */
    param_.ebwid = 1.0;		/* ebwid */
    param_.ebnum = 2.0;		/* ebnum */
    param_.sizfac = 0.0;	/* sizfac */
    param_.smooth = 0.0;	/* smooth */
    param_.spline = 0.0;	/* spline */
    param_.stem = 0.0;		/* stem */
    param_.header = 1.0;	/* header */
    param_.sort = 0.0;		/* sort */
    param_.clip = 0.0;		/* clip */
    param_.fgcol = -1.0;	/* fgcol */
    param_.bgcol = -1.0;	/* bgcol */
    param_.fncol = -1.0;	/* fncol */
    param_.norm = 0.0;		/* norm */
    param_.count = 0.0;		/* count */
    param_.cdext = 0.0;		/* cdext */
    param_.like = 0.0;		/* like */
/*
 *     common /text/
 */
    ctext_.xfmt[0] = CNULL;	/* xfmt */
    ctext_.yfmt[0] = CNULL;	/* yfmt */
    ctext_.tfmt[0] = CNULL;	/* tfmt */
    ctext_.rfmt[0] = CNULL;	/* rfmt */
    ctext_.xlabel[0] = CNULL;	/* xlabel */
    ctext_.ylabel[0] = CNULL;	/* ylabel */
    ctext_.tlabel[0] = CNULL;	/* tlabel */
    ctext_.rlabel[0] = CNULL;	/* rlabel */
    ctext_.xannot[0] = CNULL;	/* xannot */
    ctext_.yannot[0] = CNULL;	/* yannot */
    ctext_.tannot[0] = CNULL;	/* tannot */
    ctext_.rannot[0] = CNULL;	/* rannot */
    ctext_.xdata[0] = CNULL;	/* xdata */
    ctext_.ydata[0] = CNULL;	/* ydata */
    ctext_.zdata[0] = CNULL;	/* zdata */
    ctext_.select[0] = CNULL;	/* select */
}

/*
 * defflg - set flags to default values
 */
void
defflg(void)
{
/*
 *     common /flags/
 */
    flags_.finish = FALSE;
    flags_.axflg = TRUE;
    flags_.rngflg = TRUE;
    flags_.newpag = TRUE;
}
