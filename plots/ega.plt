; ega.plt
;
bgcol=7 : fncol=15
header=0 : axlwt=0 : annlwt=0
xmin=0 : ymin=0 : xmax=8 : ymax=8
xllc=0 : yllc=0 : xlen=8 : ylen=8
plot
mhal=center : mval=bottom : msgsiz=2
5.25 6 "EGA colors"


%define box ; <xllc> <yllc> <color_index> <color_name>
xofst=0.25+$1 : yofst=$2 : shdcol=$3
pltype=6 : shade=21 : xgain=1.2 : ygain=2
mhal=center : mval=half : msgsiz=0.95
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
mhal=center : mval=top : msgsiz=0.5
(0.35*xgain+xofst) (-0.05*ygain+yofst) "$4"
%%

%box 1 4  0 "Black"
%box 2 4  1 "Blue"
%box 3 4  2 "Green"
%box 4 4  3 "Cyan"
%box 5 4  4 "Red"
%box 6 4  5 "Magenta"
%box 7 4  6 "Brown"
%box 8 4  7 "Light Gray"
%box 1 2  8 "Gray"
%box 2 2  9 "Light Blue"
%box 3 2 10 "Light Green"
%box 4 2 11 "Light Cyan"
%box 5 2 12 "Light Red"
%box 6 2 13 "Light Magenta"
%box 7 2 14 "Yellow"
%box 8 2 15 "White"
