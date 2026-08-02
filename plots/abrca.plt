; PLT file created by Greg2000(2.4.0)
;
; Workbook and sheet name:  Book2 Gorga1989
; .plt file path and name:  C:\PlotPreview.plt
; Date and time created:    6/6/2011 3:41:10 PM
;
;============  PAGE    1  =============
head=0
NEWPAGE
xbreak=88
SIZFAC  =5.000
MAGNIFY =1.000
ROTATE  =0.000
VXLLC   =0.000
VYLLC   =0.000
XCYCLE = 0.000
XMIN = 40
XMAX = 200
XINT = 4.4
YCYCLE = 0.000
YMIN = 5
YMAX = 10
YINT = 5.5
GRID    =0.000
XLENGTH =5.000
YLENGTH =5.000
PLTYPE = 1.000
TICDIR  =1.000
SYMSIZ  =0.600
MSGSIZ  =1.000
LABSIZ  =1.200
ANNSIZ  =1.200
ANNLWT  =1.000
LABLWT  =1.000
PLTLWT  =1.000
ECHO    =0.000
AXLWT   =1.000
MSGLWT  =1.000
YHOR    =1.000
XANSKP  =0.000
YANSKP  =0.000
XPERCENT=70.000
YPERCENT=100
XSHIFT=-1.8
XLABEL = 
YLABEL = 
XFMT = I4
YFMT = I4
TICSIZ  =1.000

NEWFRAME
XLLC    =1.750
YLLC    =1.750
XANSKP  =0.000
YANSKP  =0.000
MXLLC   =0.250
MYLLC   =2.200
MSGSIZ  =1.400
MHALIGN = 2
MSGANGLE = 90.000
MXLLC = -0.600
MYLLC = 2.500
"Wave V Latency (ms)"
MSGANGLE = 0.000
MXLLC = 2.500 
MYLLC = -0.600
"Conceptional Age (weeks)"
MHALIGN = 0

;============= Infants ===============

pltype=lines
lintyp=4
40 5
40 10
plot
lintyp=0
ndata=159
xdata=$0+31
ydata=4.89+4.46*exp(-0.0318*$x)+5.31*exp(-0.0251*$1)
20
plot
40
plot
60
plot
80
plot
ndata=1
xdata=
ydata=
pltype=symbols

SYMBOL = 3.000
LINTYPE = 0.000
DATA
    33.678     9.726     0.000
    35.137     9.602     0.000
    37.568     9.552     0.000
    39.514     9.366     0.000
    41.459     9.303     0.000
    42.918     9.154     0.000
    58.967     8.694     0.000
    72.584     8.595     0.000
    85.714     8.296     0.000
    98.845     8.284     0.000
    111.976     8.308     0.000
    125.106     8.209     0.000
    138.237     8.035     0.000
    151.368     8.284     0.000
    163.526     7.973     0.000
    176.657     8.085     0.000
    189.787     8.097     0.000
PLOT

SYMBOL = 2.000
LINTYPE = 0.000
DATA
    33.192     8.445     0.000
    35.137     8.371     0.000
    37.568     8.259     0.000
    39.514     8.085     0.000
    41.459     8.047     0.000
    43.404     7.910     0.000
    59.453     7.401     0.000
    73.070     7.251     0.000
    85.714     7.027     0.000
    98.845     6.990     0.000
    112.462     6.978     0.000
    125.106     6.915     0.000
    138.237     6.779     0.000
    151.368     6.853     0.000
    163.526     6.729     0.000
    176.657     6.741     0.000
    189.787     6.791     0.000
PLOT

SYMBOL = 1.000
LINTYPE = 0.000
DATA
    33.678     7.587     0.000
    35.623     7.550     0.000
    38.055     7.413     0.000
    39.514     7.276     0.000
    41.945     7.177     0.000
    43.404     7.065     0.000
    58.967     6.716     0.000
    72.584     6.542     0.000
    85.714     6.306     0.000
    98.845     6.281     0.000
    111.976     6.219     0.000
    125.106     6.157     0.000
    138.237     6.107     0.000
    150.881     6.057     0.000
    163.526     6.070     0.000
    176.657     6.045     0.000
    189.301     6.057     0.000
PLOT

SYMBOL = 0.000
LINTYPE = 0.000
DATA
    33.678     7.027     0.000
    36.109     6.978     0.000
    35.623     6.990     0.000
    40.000     6.791     0.000
    41.945     6.667     0.000
    43.404     6.530     0.000
    58.967     6.244     0.000
    72.584     6.082     0.000
    85.714     5.883     0.000
    98.845     5.908     0.000
    112.462     5.833     0.000
    125.106     5.709     0.000
    137.751     5.709     0.000
    150.881     5.709     0.000
    163.526     5.572     0.000
    176.170     5.647     0.000
    189.301     5.672     0.000
PLOT

;=========== Adults ===============

newrange
xanskp=-1
yanskp=-1
xper=13.2
xshift=(xper-100)/2
xint=1.3
plot
newrange
xanskp=0
xper=7
xshift=46.5
xmin=900
xmax=1000
xint=1
xanno=900,A

pltype=lines
lintyp=0
ndata=2
xdata=$0*100+750
ydata=4.89+4.46*exp(-0.0318*$x)+5.31*exp(-0.0251*$1)
20
plot
40
plot
60
plot
80
plot
ndata=1
xdata=
ydata=
pltype=symbols

SYMBOL = 3.000
LINTYPE = 0.000
DATA
    900     7.910     0.000
PLOT

SYMBOL = 2.000
LINTYPE = 0.000
DATA
    900     6.667     0.000
PLOT

SYMBOL = 1.000
LINTYPE = 0.000
DATA
    900     5.933     0.000
PLOT

SYMBOL = 0.000
LINTYPE = 0.000
DATA
    900     5.547     0.000
PLOT

;====================================

msgdat=n : msgsiz=0.8
3.30 4.70 "
|3| 20 dB HL[n]
|2| 40
|1| 60
|0| 80
"
