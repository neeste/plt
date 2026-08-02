;
;   DELTA F STUDY - AUDIOGRAMS
;
;   .PLT FILE NAME: DFAUD.PLT
;
;   DATE CREATED:   27-SEP-84
;
;============  PAGE    1  =============
;
filcol=14
YHOR=Y
clip=yes
HEADER=NO
XLENGTH=6.4
YLENGTH=6.4
XMIN=0
XMAX=6
XINT=6.1
YMIN=-110
YMAX=10
YINT=12.1
YLABEL=hearing level (re ANSI - 1969)
XANSKP=-1.0
tmin=125
tcyc=6
tint=-2
tanskp=0
tlabel=frequency in Hz
TICDIR=IN
SYMSIZ=1.25
MSGSIZ=1.400
LABSIZ=1.0
ANNSIZ=1.0
XFMT=I4
YFMT=I4
XLLC=2.5
YLLC=.5
YANSKP=1.1
YHOR=YES
PLOT

MXLLC=6.75
MYLLC=-10
"NDH"
PLOT

PLTYPE=B
SYMBOL=11.000
LINTYPE=0.000
;NDH
DATA
1,-5
2,0
3,-5
4,-5
5,-10
6,-15
PLOT
