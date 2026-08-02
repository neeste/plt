; arrows.plt

%define arrow ; <xtail> <ytail> <xhead> <yhead> <head_size>
pltype=line : lintyp=0 : aronum=1 : arosiz=$5*6.6 : arotip=0.5
($1) ($2)
($3) ($4)
plot
%%

head=0 : axlwt=0 : annlwt=0 : xmax=(xlen) : ymax=(ylen) : xllc=0 : yllc=0
pltlwt=2

%arrow 5 4 1 1 0.25
%arrow 5 4 2 1 0.25
%arrow 5 4 3 1 0.25
%arrow 5 4 4 1 0.25
%arrow 5 4 5 1 0.25
%arrow 5 4 6 1 0.25
%arrow 5 4 7 1 0.25
%arrow 5 4 8 1 0.25
%arrow 5 4 9 1 0.25
%arrow 5 4 9 2 0.25
%arrow 5 4 9 3 0.25
%arrow 5 4 9 4 0.25
%arrow 5 4 9 5 0.25
%arrow 5 4 9 6 0.25
%arrow 5 4 9 7 0.25
%arrow 5 4 8 7 0.25
%arrow 5 4 7 7 0.25
%arrow 5 4 6 7 0.25
%arrow 5 4 5 7 0.25
%arrow 5 4 4 7 0.25
%arrow 5 4 3 7 0.25
%arrow 5 4 2 7 0.25
%arrow 5 4 1 7 0.25
%arrow 5 4 1 6 0.25
%arrow 5 4 1 5 0.25
%arrow 5 4 1 4 0.25
%arrow 5 4 1 3 0.25
%arrow 5 4 1 2 0.25
