head=0 : xlen=4 : ylen=3.5 : msgsiz=1.5 : ticdir=in
labsiz=msgsiz : annsiz=msgsiz
pltyp=lines
ndata=99
xdata=$0
ydata=$1*(10^($2*(100-$x))-$3)
zdata=0
xmin=0 : xmax=100 : xper=90
ymin=0.01 : ymax=100 : ycyc=4 : yhor=1
xlab=place (% distance from base)
ylab=frequency (kHz)
%define map
lintyp=$4 : pltl=$5
$1 $2 $3 
plot
%%
%map 0.1654 0.021 0.99 0 1	; man
%map 0.1635 0.021 0.85 2 2	; chinchilla
%map 0.3500 0.021 0.85 4 1	; guinea pig
%map 0.4000 0.021 0.85 6 1	; gerbil
%map 0.4560 0.021 0.80 8 1	; cat
%map 5.0000 0.011 0.90 10 1	; rat

pltl=1
0.5 1.7 "
|_0| man

|_4| guinea pig
|_6| gerbil
|_8| cat
|_10| rat
"
pltl=2
0.5 1.7 "

|_2| chinchilla
"
; add symbols
;pltyp=symb : symb=1 : pltcol=4
;ndat=1 : xdat=$4
;0.1654 0.021 0.99 33.3 ; %
;0.1654 0.021 0.99 71.3 ; %
;xdat=100-log(($4/$1)+$3)/$2
;0.1654 0.021 0.99 4.0 ; kHz
;0.1654 0.021 0.99 0.5 ; kHz
;datl
;plot
