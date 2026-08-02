; vbc2.plt - another vertical bar chart demo using macros

%define VBSETUP ; <# bin groups> <bins/group> <group per.> <x per.>
pltype=r : mxdat=y : mydat=n : mhal=c : mval=t : m=-0.1 : g=$1 : b=$2
xtick=0 : xper=$4 : xmin=0.5 : xmax=g+0.5
w=($3)/($2)/100
%%

%define VBAR ; <group #> <dat1> ... <dat7>
x=($1)-3.5*w
shade=20
(x)+0*w 0
(x)+1*w ($2)
plot
shade=21
(x)+1*w 0
(x)+2*w ($3)
plot
shade=22
(x)+2*w 0
(x)+3*w ($4)
plot
shade=23
(x)+3*w 0
(x)+4*w ($5)
plot
shade=24
(x)+4*w 0
(x)+5*w ($6)
plot
shade=25
(x)+5*w 0
(x)+6*w ($7)
plot
shade=26
(x)+6*w 0
(x)+7*w ($8)
plot
%%

openax=y : xlen=6
xann=1,P13 2,P15 3,P17 4,P18
xlab=Age (postnatal days)
yhor=y : ylen=3 : ymax=1.0 : yfmt=f.1
ylab=Proportion Responding

%VBSETUP 4 7 80 100
%VBAR  1 0.15 0.15 0.38 0.55 0.55 0.55 0.55 ; P13
%VBAR  2 0.20 0.57 0.50 0.78 0.87 0.85 0.87 ; P15
%VBAR  3 0.58 0.58 1.00 1.00 1.00 1.00 1.00 ; P17
%VBAR  4 1.00 1.00 1.00 1.00 1.00 1.00 1.00 ; P18

msgdat=0
1 3 "|#20|  1.3 kHz
|#21|  1.8 kHz
|#22|  2.6 kHz
|#23|  3.9 kHz
|#24|  5.2 kHz
|#25|  9.7 kHz
|#26| 13.0 kHz
"
