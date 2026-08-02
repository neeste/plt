; grays.plt
;
header=0 : axlwt=0 : annlwt=0
xmin=0 : ymin=0 : xmax=8 : ymax=8
xllc=0 : yllc=0 : xlen=8 : ylen=8
plot
mhal=center : mval=bottom : msgsiz=2
5.25 6 "Grays"

pltype=6 : shade=21 : a=0.25 : pltcol=0 : pltcol=0
mhal=center : mval=half : msgsiz=0.5*xgain
shade=60

%define box ; <xllc> <yllc> <gray>
xgain=1.5 : ygain=1.5
xofst=0.35+1.1*$1 : yofst=1.5*$2 : shdgray=$3
(0.35*xgain+xofst) (0.35*ygain+yofst) "$3"
0.0 0.0
0.0 0.7
0.7 0.7
0.7 0.0
0.4 0.0
0.4 0.2
0.5 0.2
0.5 0.5
0.2 0.5
0.2 0.2
0.3 0.2
0.3 0.0
plot
%%

%box 1 3 0.00
%box 2 3 0.05
%box 3 3 0.10
%box 4 3 0.15
%box 5 3 0.20
%box 6 3 0.25
%box 7 3 0.30
%box 1 2 0.35
%box 2 2 0.40
%box 3 2 0.45
%box 4 2 0.50
%box 5 2 0.55
%box 6 2 0.60
%box 7 2 0.65
%box 1 1 0.70
%box 2 1 0.75
%box 3 1 0.80
%box 4 1 0.85
%box 5 1 0.90
%box 6 1 0.95
%box 7 1 1.00

