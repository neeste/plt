;-------------------------->>> Begin <<<-------------------------------
;
;                     PLT File Name: scatter.plt
;
;  (Example of a plt file to make a plot with points only (scattergram.)
;
;         Name:    W.J. Kelly           Date: 16-Feb-89
;
;-------------->>>  Description of Frame and Axes  <<<-----------------
;
xlength =  6.0   : ylength =  6.0  : xllc    =  1.5  : yllc    =   1.0
xmin    =  0.0   : xmax    = 20.0  : ymin    =  0.0  : ymax    =  20.0
xint    =  4.5   : yint    =  4.5  : xpercent=100.0  : ypercent= 100.0
yhor    = YES    : xfmt    =  I2   : yfmt    =  I2   : ticdir  = INWARD
xlabel  =  Chronological Age (Yrs)
ylabel  =  Test Score Age (Yrs)
msgdat  =  NO
;--------------------->>>  Title of Plot  <<<--------------------------
;
mhalign = CENTER : msgsiz  =  1.1   : mxllc   =  3.0  : myllc   =   6.2
"PEABODY PICTURE VOCABULARY TEST"
plot
;---------------------------------------------------
;              >>>  Other messages  <<<
;
mhalign = LEFT  : msgdat  =  YES
msgsiz  =  1.0   : mxllc   =  2.0  : myllc   = 18.0
"
|11|  Severe loss (N = 32)

|7|  Profound Loss (N = 40)
"
plot
;---------------------------------------------------
; >>>  Draw a diagonal line with slope 1.0 through the origin.  <<<
;
;
;
pltype  = 2.0    : symbol  =  0.0  : lintype =  0.0
data
0.,0.
20.0,20.0
plot
;---------------------------------------------------
;              >>>  Data Section  <<<
;
pltype  = 1.0    : symbol  = 11.0  : lintype =  0.0
;
data  ; Scores for severe loss.
;
;Age at Test    Age Score
;------------------------
   13.42         8.58
   10.92         4.00
    7.17         4.50
   10.83         9.67
    8.17         5.17
    9.83         8.50
    9.92         6.83
    8.42         6.83
   17.33        13.25
    7.33         3.67
    6.75         4.67
    9.17         5.67
    2.92         2.83
    7.08         2.08
    6.08         3.42
    6.75         6.08
    5.17         2.83
   17.83         4.17
    5.92         6.25
    4.17         2.00
    4.83         2.67
    4.92         3.33
    6.58         2.83
    6.42         4.00
    6.50         5.25
    4.50         3.00
   11.00         7.83
   12.58         9.67
    3.50         2.83
   17.75         8.92
   11.83         7.67
    9.08         3.58
plot
;---------------------------------------------------
pltype  = 1.0    : symbol  =  7.0  : lintype =  0.0
;
data  ; Scores for profound loss.
;
;Age at Test    Age Score
;------------------------
   10.58         5.42
   14.25         6.83
    7.17         5.58
    5.83         3.33
    4.92         3.08
    8.33         4.08
    5.83         7.25
   11.83         9.83
    8.33         7.92
    5.25         5.83
   12.58         7.17
    6.92         5.17
    4.25         2.83
    4.17         3.25
    4.67         3.08
    7.58         5.83
   11.75         7.17
    8.92         5.08
    7.92         7.25
    8.08         5.25
   14.33         9.42
    5.75         3.00
    9.00         6.25
   17.50         6.25
    8.42         6.00
    9.00         3.83
   15.00         3.83
    5.83         2.75
    4.33         2.08
    5.33         4.08
   10.75         7.67
    3.33         2.08
   15.25        10.08
    4.42         4.50
    5.75         3.08
    8.50         3.17
    6.25         3.58
    6.75         4.00
    6.50         3.92
    7.42         4.58
plot
finish
;----------------------- End -----------------------

