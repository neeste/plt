; mp.plt - Mecamylamine Pump Study (3 Day)
;       
;****************************************************
;
HEAD=no : OPENAX=yes : TICDIR=out : YHOR=yes
XLEN=8 : XLLC=1.9 : XMIN=0 : XMAX=20.0 : XINT=0  : XFMT=I3   : XANSKIP=-1
YLEN=6 : YLLC=1.4 : YMIN=0 : YMAX=0.14 : YINT=14 : YFMT=F6.2 : YANSKIP=1
XLABEL=
YLABEL=pmol/mg protein
;
MSGSIZ=1 : MHALIGN=C : MVALIGN=H : MSGDAT=Y
4. -.01 "Cortex"
10 -.01 "Hypothalamus"
16 -.01 "Hippocampus"

msgsiz=0.8 : mhalign=l : mvalign=h : mhkey=3   : mvkey=2 : mvsp=2
1 .13 "
|#20| control
|#21| 1 mg/kg/day
|#38| 2 mg/kg/day
|#32| 5 mg/kg/day"

msgsiz=1.0 : mvalign=b : mhalign=c
10 .14 "Mecamylamine Pump Study (3 Day)"
pltype=4 : shade=0 : ebnum=1
0
0
.065,.002
0
0
0
0
0
.113,.002
0
0
0
0
0
.123,.006
0
0
0
0
0
plot

shade=21
0
0
0
.069,.002
0
0
0
0
0
.115,.004
0
0
0
0
0
.112,.005
0
0
0
0
plot

shade=38
0
0
0
0
.068,.004
0
0
0
0
0
.116,.004
0
0
0
0
0
.104,.003
0
0
0
plot

shade=32
0
0
0
0
0
.063,.002
0
0
0
0
0
.098,.009
0
0
0
0
0
.103,.010
0
0
plot

0
plot

