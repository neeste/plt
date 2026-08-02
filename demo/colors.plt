; colors.plt
;
header=0 : axlwt=0 : annlwt=0
xmin=0 : ymin=0 : xmax=8 : ymax=8
xllc=0 : yllc=0 : xlen=8 : ylen=8

mhal=center : mval=bottom : msgsiz=2
5.25 6 "Colors"

include plt.pal		; define a new color palette

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

%box 1 4  0 "black"
%box 2 4  1 "blue"
%box 3 4  2 "green"
%box 4 4  3 "cyan"
%box 5 4  4 "red"
%box 6 4  5 "magenta"
%box 7 4  6 "brown"
%box 8 4  7 "lt. gray"
%box 1 2  8 "dk. gray"
%box 2 2  9 "lt. blue"
%box 3 2 10 "lt. green"
%box 4 2 11 "lt. cyan"
%box 5 2 12 "lt. red"
%box 6 2 13 "lt. magenta"
%box 7 2 14 "yellow"
%box 8 2 15 "white"
