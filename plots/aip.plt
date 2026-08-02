; aip.plt

%define tt
symbol=$1
ydat=$2
data
 3 15.086 15.337 15.413
 6 15.380 15.443 15.565
12 15.169 15.648 15.823
24 14.956 15.778 15.960
48 14.728 15.914 16.067
96 14.759 15.907 16.211
plot
%%

%define arrow
pltype=line : lintyp=0 : aronum=1 : arosiz=$5*6.6 : arotip=0.5
($1) ($2)
($3) ($4)
plot
%%

header=no : ticdir=in : yhor=yes
xlen=6.18 : ylen=5.20 : xper=83.4
xmin=3 : xmax=96 : xcyc=5 : xint=-2
ymin=14.5 : ymax=16.5 : yint=4 : yfmt=f.1
;xannot=3 6 12 24 48 96
xlabel=irradiation time (s)
ylabel=transition temperature (K)
pltype=both : zdat=0

%tt 0 $2
%tt 2 $3
%tt 1 $4

msgdat=0
0.48 0.50 "(a)"
1.95 0.80 "Cu[88]Nb[10]Sn[2]"
4.00 2.60 "Cu[87.5]Nb[10]Sn[2.5]"
1.20 4.00 "Cu[87]Nb[10]Sn[3]"

newframe
annlwt=0 : axlwt=0 : lablwt=0
xmin=0 : xmax=(xlen) : xper=100 : xcyc=0 
ymin=0 : ymax=(ylen) : yper=100 : ycyc=0 
pltype=lines : ydat=
%arrow 3.33 0.84 3.85 0.95 0.1
%arrow 3.93 2.70 3.40 3.16 0.1
%arrow 2.55 4.00 2.87 3.63 0.1
