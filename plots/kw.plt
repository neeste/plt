;
;   DELTA F STUDY - AUDIOGRAMS
;
;   .PLT FILE NAME: DFAUD.PLT
;
;   DATE CREATED:   27-SEP-84
;
;============  PAGE    1  =============
;
HEADER=no
XLENGTH=3.8 : XMIN=0   : XMAX=6   : XINT=6.1  : XFMT=I4 : XLLC=.9 : XANSKP=-1.0
YLENGTH=3.8 : YMIN=130 : YMAX=-10 : YINT=14.1 : YFMT=I4 : YLLC=.5 : YANSKP=1.1
YHOR=YES
YLABEL=HEARING LEVEL (re ANSI - 1969)
;GRID=YES
TICDIR=IN
SYMSIZ=1.25
MSGSIZ=1.400
LABSIZ=1.0
ANNSIZ=1.0
MSGDAT=YES
tans=0
tmin=125
tcyc=6
;tmax=8000
tint=-2
tlab=FREQUENCY in Hz

MXLLC=2
MYLLC=-60
" Left - 6 months
Post 2nd Op (right)
   Subject:  KW"
PLOT

PLTYPE=poly
SHADE=14
sizfac=2
DATA
1,5
2,5
3,25
4,20
5,45
6,90
6,105
5,75
4,40
3,45
2,35
1,10
keep
pltype=symbol
symbol=10
symsiz=.25
PLOT
