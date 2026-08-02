; sufi.plt

%define SQUARE ; <x-center> <y-center> <square-side>
x=$1 : y=$2 : z=$3/(2*(x*x+y*y))^0.5 : u=z*$1 : v=z*$2
(x-u) (y-v)
(x-v) (y+u)
(x+u) (y+v)
(x+v) (y-u)
plot
%%
%define DISPSUFI ; <x-center> <y-center> <square-side> <gutter>
pltyp=poly : xofs=$1 : yofs=$2 : a=($3+$4)/2 : b=a*2^0.5 : s=$3
%SQUARE  a  a  s
%SQUARE  0  b  s
%SQUARE -a  a  s
%SQUARE -b  0  s
%SQUARE -a -a  s
%SQUARE  0 -b  s
%SQUARE  a -a  s
%SQUARE  b  0  s
%%

annl=0 : axlw=0 : head=0 : xllc=0 : xmax=6 : xlen=6 : yllc=0 : ymax=6 : ylen=6
%DISPSUFI 4 4 2 0.4     ; INVOCATION
