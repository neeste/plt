; nhc.plt
;
clip=1
xdata=$1/1000
ydata=$2
XCYCLE  =    2.000
XMIN    =    0.2500
XMAX    =    8.0000
XINT    =    -2.   
YCYCLE  =     1.000
YMIN    =     2.000
YMAX    =     8.000
YINT    =     8.   
GRID    =     0.   
XLENGTH =     5.000
YLENGTH =     5.000
PLTYPE  =     1.000
TICDIR  =     1.000
SYMSIZ  =     1.000
MSGSIZ  =     -.13
LABSIZ  =     -.18
ANNSIZ  =     -.15
ANNLWT  =     1.000
LABLWT  =     1.000
PLTLWT  =     1.000
AXLWT   =     1.000
MSGLWT  =     1.000
YHOR    =     1.000
XANSKP  =     0.   
YANSKP  =     0.   
XPERCENT=    80.000
YPERCENT=    90.000
XLABEL  =frequency (kHz)
YLABEL  =Q[ERB]
XANNOT  = 0.25 0.5 1 2 4 8
YANNOT  = 2 3 4 5 6 7 8
NEWFRAME
XLLC    =     1.750
YLLC    =     1.750
zdata=0
%define log2 (log($1)/log(2))
;
; symbols
PLTYPE  =     symb
SYMBOL  =     2
 500 4.0
4000 7.0
PLOT
SYMBOL  =     0
 500 3.5
4000 6.3
PLOT
SYMBOL  =     1
 500 3.0
4000 5.7
PLOT
SYMBOL  =     3
 500 2.5
4000 5.0
PLOT
;
; lines
PLTYPE  =     line
T1=11.9 ; dB SPL at  500 Hz
T2= 4.4 ; dB SPL at 4000 Hz
b=15.4 : d=0.41 : c2=17.7 : d2=2/3
ydata=(b*[($1/1000)*2^(-$2/c2)]^d)^d2
 500 20+T1
4000 20+T2
PLOT
L2=30
 500 30+T1
4000 30+T2
PLOT
L2=40
 500 40+T1
4000 40+T2
PLOT
 500 50+T1
4000 50+T2
PLOT

MSGSIZ  =     -.13
MXLLC   =     3.300
MYLLC   =     1.3
"
|2|   20
|0|   30
|1|   40
|3|   50 dB SPL
"
PLOT
