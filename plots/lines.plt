;-------------------------->>> Begin <<<-------------------------------
;
;                     PLT File Name: lines.plt
;
;      (Example of a plt file to make a plot with lines only.)
;
;         Name:    W.J. Kelly           Date: 15-Feb-89
;
;-------------->>>  Description of Frame and Axes  <<<-----------------
;
xlength =  8.0   : ylength =  5.0  : xllc    =  1.5  : yllc    =   1.5
xmin    =  5.0   : xmax    =  8.0  : ymin    =  0.0  : ymax    = 100.0
xint    =  3.2   : yint    =  5.2  : xpercent= 80.0  : ypercent= 100.0
yhor    = YES    : xfmt    =  I2   : yfmt    =  I2   : ticdir  = INWARD
xlabel  =  Age (Years)
ylabel  =  Mean Percent Correct
msgdat  =  NO
;--------------------->>>  Title of Plot  <<<--------------------------
;
mhalign = CENTER : msgsiz  =  1.1   : mxllc   =  4.0  : myllc   =   5.2
" SPOKEN LANGUAGE OF DEAF CHILDREN"
plot
;---------------------------------------------------
;              >>>  Other messages  <<<
;
mhalign = LEFT  : msgdat  =  YES
msgsiz  =  1.0   : mxllc   =  5.0  : myllc   = 43.0
" Oral Children"
msgsiz  =  1.0   : mxllc   =  5.0  : myllc   = 13.0
" Total Communication Children"
plot
;---------------------------------------------------
;              >>>  Data Section  <<<
;
pltype  = 2.0    : symbol  = 10.0  : lintype =  0.0
;
data       ;  Data for oral children.
5,40
6,40
7,50
8,65
plot
;---------------------------------------------------
pltype  = 2.0    : symbol  = 10.0  : lintype = 10.0
;
data     ;  Data for total communication children.
5,20
6,20
7,35
8,23
plot
finish
;----------------------- End -----------------------

