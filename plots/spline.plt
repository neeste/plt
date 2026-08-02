;-------------------------->>> Begin <<<-------------------------------
;
;                     PLT File Name: spline.plt
;
;  (Example of a plt file with a spline fit to points with error bars.)
;
;         Name:    S. Neely             Date: 15-Feb-89
;
;-------------->>>  Description of Frame and Axes  <<<-----------------
;
xlength =  6.0   : ylength =  6.0  : xllc    =  1.5  : yllc    =   1.0
xmin    =  0.0   : xmax    =  0.1  : ymin    =  0.0  : ymax    =   0.5
xint    =  3.2   : yint    =  5.2  : xpercent= 80.0  : ypercent= 100.0
yhor    = YES    : xfmt    = f3.2  : yfmt    = f3.1  : ticdir  = INWARD
xlabel  =  X - Axis Label
ylabel  =  Y - Axis Label
;------------------->>>  Misc Parameters  <<<--------------------------
spline=20        : ebnum = 2
msgdat  =  NO
;--------------------->>>  Title of Plot  <<<--------------------------
;
mhalign = CENTER : msgsiz  =  1.1   : mxllc   =  3.0  : myllc   =   6.2
"SPLINE FIT TO DATA POINTS"
plot
;---------------------------------------------------
;              >>>  Other messages  <<<
;
mhalign = LEFT  : msgdat  =  YES
msgsiz  =  1.0   : mxllc   =  5.0  : myllc   = 43.0
""
plot
;---------------------------------------------------
;              >>>  Data Section  <<<
;
pltype  = 3.0    : symbol  = 11.0  : lintype =  0.0
;
data
;     X          Y      Error
;  ------      ----     -----
   0.0200,     0.07,    .05
   0.0238,     0.08
   0.0283,     0.10,    .05
   0.0336,     0.13
   0.0400,     0.18,    .05
   0.0476,     0.28
   0.0566,     0.27,    .05
   0.0673,     0.11
   0.0800,     0.16,    .05
   0.0951,     0.22
plot
finish
;----------------------- End -----------------------

