; barchart.plt - vertical bar chart demo using macros

%define VBSETUP ; <number of bins> <percent rectangle width> <x-axis percent>
pltype=r : mxdat=y : mydat=n : mhal=c : mval=t : m=-0.1
xtick=0 : xans=-1 : xper=$3 : xmin=0 : xmax=($1)+1 : w=($2)/200
%%

%define VBAR ; <ordinal position> <data value> <bar label>
ebnum=1 : e=0.1+0.4*($$random+$$random) ; add random-sized error bars
$1-w 0
$1+w $2 e
plot
$1 m "$3"
%%

openax=y : yhor=y : ylen=5 : ymax=10
%VBSETUP 10 50 80
%VBAR  1 3 "A"
%VBAR  2 7 "B"
%VBAR  3 6 "C"
%VBAR  4 4 "D"
%VBAR  5 3 "E"
%VBAR  6 2 "F"
%VBAR  7 1 "G"
%VBAR  8 3 "H"
%VBAR  9 2 "I"
%VBAR 10 5 "J"
