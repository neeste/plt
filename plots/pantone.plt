; pantone.plt
;
; specify a new color palette:
;
%define CMYK "(1-($1+$4)/100) (1-($2+$4)/100) (1-($3+$4)/100)"
data
%CMYK    0     0     0   100      ; color  0: Black
%CMYK    0    23.5  15     0      ; color  1: Pantone 176 C
%CMYK    0    47    43     0      ; color  2: Pantone 177 C
%CMYK    0    60    56     0      ; color  3: Pantone 178 C
%CMYK    0    79    91     0      ; color  4: Pantone Warm Red C
%CMYK  100     0    65     0      ; color  5: Pantone Green
%CMYK  100     0   100     0      ; color  6: RGB Green
%CMYK    0     0     0     0      ; color  7: White
%CMYK    0     0   100     0      ; color  8: Pantone Yellow
%CMYK    0   100     0     0      ; color  9: Pantone Magenta 
%CMYK  100     0     0     0      ; color 10: Pantone Cyan
%CMYK    0     0     0   100      ; color 11: Black
%CMYK    0    51    87     0      ; color 12: Orange
%CMYK    0    91    87     0      ; color 13: Red
%CMYK  100    79     0     0      ; color 14: Blue
%CMYK  100   100     0     0      ; color 15: RGB Blue
palette
;
header=0 : axlwt=0 : annlwt=0 : bgcol=7
xmin=0 : ymin=0 : xmax=8 : ymax=8
xllc=0 : yllc=0 : xlen=8 : ylen=8
plot
mhal=center : mval=bottom : msgsiz=2
5.25 6 "Pantone Colors"
;
; draw box around each color
;
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
;
%box 1 4  0 "Black"
%box 2 4  1 "176"
%box 3 4  2 "177"
%box 4 4  3 "178"
%box 5 4  4 "Warm Red"
%box 6 4  5 "Green"
%box 7 4  6 "GREEN"
%box 8 4  7 "White"
%box 1 2  8 "Yellow"
%box 2 2  9 "Magenta"
%box 3 2 10 "Cyan"
%box 4 2 11 "Black"
%box 5 2 12 "Orange"
%box 6 2 13 "Red"
%box 7 2 14 "Blue"
%box 8 2 15 "BLUE"
