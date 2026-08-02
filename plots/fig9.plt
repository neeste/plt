; fig9.plt - Example 3
;
rotate=yes : wylen=10.5 : head=0 : mhal=center
1.8 -0.75 "Fig. 9.  Example 3"
vyllc=1 : wylen=9.5 : magnify=0.6 : mhal=0
;
; Mecamylamine Pump Study (3 Day)
;
head=no : openax=yes : ticdir=out : yhor=yes 
xlen=8 : xllc=1.9 : xmin=2 : xmax=18 : xper=80
ylen=6 : yllc=1.4 : ymin=0 : ymax=0.14
xint=0  : xfmt=i3   : xanskip=-1
yint=14 : yfmt=f6.2 : yanskip=1
ylabel=pmol/mg protein

%define bars ; <order> <shade> <data1> <data2> <data3>
pltype=histo : ebnum=1 : shade=$2
%repeat ($1-1) 0
$3
%repeat 5 0
$4
%repeat 5 0
$5
%repeat (4-$1) 0
plot
%%

;               Cortex     Hypothalamus  Hippocampus
%bars 1 20   0.065,0.002   0.113,0.002   0.123,0.006  ; control
%bars 2 21   0.069,0.002   0.115,0.004   0.112,0.005  ; 1 mg/kg/day
%bars 3 38   0.068,0.004   0.116,0.004   0.104,0.003  ; 2 mg/kg/day
%bars 4 32   0.063,0.002   0.098,0.009   0.103,0.010  ; 5 mg/kg/day

msgsiz=1 : mhalign=c : mvalign=h : msgdat=y
4. -.01 "Cortex"
10 -.01 "Hypothalamus"
16 -.01 "Hippocampus"
msgsiz=0.8 : mhalign=l : mvalign=h : mhkey=3   : mvkey=2 : mvsp=2
1 .13 "
|#20| control
|#21| 1 mg/kg/day
|#38| 2 mg/kg/day
|#32| 5 mg/kg/day"
msgsiz=1.2 : mvalign=b : mhalign=c
10 .14 "Mecamylamine Pump Study (3 Day)"
