; me_circuit.plt - middle-ear analog circuits
;
axlwt=0 : annlwt=0 : head=0
xllc=0 : yllc=0
xlen=10.5 : ylen=8
xmax=10.5 : ymax=8
mhal=c : mval=b : msgsiz=1
;
;----------------------- circuit elements ----------------
;
; terminals
%define term ; <x1> <y1> <x2> <y2>
pltlwt=1 : pltype=symbols : symsiz=0.7
($1) ($2)
($3) ($2)
plot
($1) ($4)
($3) ($4)
plot
%%
; wires
%define wire ; <x1> <y1> <x2> <y2>
pltlwt=1 : pltype=line
($1) ($2)
($3) ($4)
plot
%%
;
;----------------------- vertical circuit elements ----------------
;
; resistor - vertical
%define vres ; <x1> <y1> <y2> <width> <height>
pltlwt=1 : pltype=line
_dy=($5)/7 : _dx=($4)/2
_x1=$1+_dx : _x2=$1-_dx
_y1=($2+$3-$5-_dy)/2 : _y2=($2+$3+$5-_dy)/2
($1)      ($2)
($1)      (_y1)
(_x2) (_y1+_dy*1)
(_x1) (_y1+_dy*2)
(_x2) (_y1+_dy*3)
(_x1) (_y1+_dy*4)
(_x2) (_y1+_dy*5)
(_x1) (_y1+_dy*6)
($1)      (_y2)
($1)      ($3)
plot
%%; inductor - vertical
%define vind ; <x1> <y1> <y2> <width> <height>
pltlwt=1 : pltype=line
_y1=($2+$3-$5)/2 : _y2=($2+$3+$5)/2
($1) ($2)
($1) (_y1)
rx=($4)/2 : ry=($4)/5 : _dy=($5-2*ry) : ndat=400 : tph=3.1415927*7
xdata=$$1+rx*sin(tph*(($$0-1)/(ndata+1)))
ydata=$$2-ry*cos(tph*(($$0-1)/(ndata+1)))+ry+_dy*(($$0-1)/(ndata+1))
($1) (_y1)
ndata=1 : xdata= : ydata=
($1) (_y2)
($1) ($3)
plot
%%
; capacitor - vertical
%define vcap ; <x1> <y1> <y2> <width> <height>
pltlwt=1 : pltype=line
_x1=($1-$4/2)    : _x2=($1+$4/2) 
_y1=($2+$3-$5)/2 : _y2=($2+$3+$5)/2
($1)  ($2)
($1)  (_y1)
plot
(_x1) (_y1)
(_x2) (_y1)
plot
(_x1) (_y2)
(_x2) (_y2)
plot
($1)  (_y2)
($1)  ($3)
plot
%%
%%; inductor - vertical
%define vind ; <x1> <y1> <y2> <width> <height>
pltlwt=1 : pltype=line
_y1=($2+$3-$5)/2 : _y2=($2+$3+$5)/2
($1) ($2)
($1) (_y1)
rx=($4)/2 : ry=($4)/5 : _dy=($5-2*ry) : ndat=400 : tph=3.1415927*7
xdata=$$1+rx*sin(tph*(($$0-1)/(ndata+1)))
ydata=$$2-ry*cos(tph*(($$0-1)/(ndata+1)))+ry+_dy*(($$0-1)/(ndata+1))
($1) (_y1)
ndata=1 : xdata= : ydata=
($1) (_y2)
($1) ($3)
plot
%%
%%; transfmormer - vertical
%define vtrn ; <x1> <x2> <y1> <y2> <width> <height>
pltlwt=1 : pltype=line
rx=($5)/2 : ry=($5)/5 : _dy=($6-2*ry) : tph=6.2832*3.5
_x1=($1) : _y1=($3+$4-$6)/2 : _y2=($3+$4+$6)/2 : _dx=($2-$1-$5*5/4)/2
(_x1) ($3)
(_x1) (_y1)
ndata=400
xdata=$$1+rx*sin(tph*(($$0-1)/(ndata+1)))
ydata=$$2-ry*cos(tph*(($$0-1)/(ndata+1)))+ry+_dy*(($$0-1)/(ndata+1))
(_x1+_dx) (_y1)
ndata=1 : xdata= : ydata=
(_x1) (_y2)
(_x1) ($4)
plot
_x1=($2) : _y1=($3+$4-$6)/2 : _y2=($3+$4+$6)/2
(_x1) ($3)
(_x1) (_y1)
ndata=400
xdata=$$1-rx*sin(tph*(($$0-1)/(ndata+1)))
ydata=$$2-ry*cos(tph*(($$0-1)/(ndata+1)))+ry+_dy*(($$0-1)/(ndata+1))
(_x1-_dx) (_y1)
ndata=1 : xdata= : ydata=
(_x1) (_y2)
(_x1) ($4)
plot
%%
; transmission line - vertical
%define htln ; <x1> <y1> <x2> <y2>
pltlwt=2 : pltype=line
($1) ($2)
($1) ($4)
plot
($3) ($2)
($3) ($4)
plot
pltlwt=1 : pltype=symbols : symsiz=0.7
($1) ($2)
($1) ($4)
plot
($3) ($2)
($3) ($4)
plot
%%
;
;----------------------- horizontal circuit elements ----------------
;
; resistor - horizontal
%define hres ; <x1> <x2> <y1> <width> <height>
pltlwt=1 : pltype=line
_dx=($4)/7 : _dy=($5)/2
_x1=($1+$2-$4-_dx)/2 : _x2=($1+$2+$4-_dx)/2
_y1=($3-_dy) : _y2=($3+_dy)
($1)        ($3) 
(_x1)       ($3)
(_x1)       ($3)    
(_x1+_dx*1) (_y2) 
(_x1+_dx*2) (_y1) 
(_x1+_dx*3) (_y2) 
(_x1+_dx*4) (_y1) 
(_x1+_dx*5) (_y2) 
(_x1+_dx*6) (_y1) 
(_x2)       ($3)
($2)        ($3) 
plot
%%
; capacitor - horizontal
%define hcap ; <x1> <x2> <y1> <width> <height>
pltlwt=1 : pltype=line
_x1=($1+$2-$4)/2 : _x2=($1+$2+$4)/2
_y1=($3-$5/2) : _y2=($3+$5/2)
($1)  ($3) 
(_x1) ($3)
plot
(_x1) (_y1)
(_x1) (_y2)
plot
(_x2) (_y1)
(_x2) (_y2)
plot
(_x2) ($3)
($2)  ($3) 
plot
%%
; inductor - horizontal
%define hind ; <x1> <x2> <y1> <width> <height>
pltlwt=1 : pltype=line
_x1=($1+$2-$4)/2 : _x2=($1+$2+$4)/2 
($2)  ($3) 
(_x2) ($3)
rx=($5)/5 : ry=($5)/2 : _dx=($4-2*rx) : ndat=400 : tph=6.2832*3.5
xdata=rx*cos(tph*(($$0-1)/(ndata+1)))+$$1-rx-_dx*(($$0-1)/(ndata+1))
ydata=ry*sin(tph*(($$0-1)/(ndata+1)))+$$2
(_x2) ($3)
ndata=1 : xdata= : ydata=
(_x1) ($3)
($1)  ($3) 
plot
%%
; transmission line - horizontal
%define htln ; <x1> <y1> <x2> <y2>
pltlwt=2 : pltype=line
($1) ($2)
($3) ($2)
plot
($1) ($4)
($3) ($4)
plot
pltlwt=1 : pltype=symbols : symsiz=0.7
($1) ($2)
($3) ($2)
plot
($1) ($4)
($3) ($4)
plot
%%
;
;---------------- 7 parameter middle-ear model --------------------
;
%wire 2.0 4.5 5.0 4.5
%wire 5.4 4.5 6.0 4.5
%wire 5.4 6.0 6.0 6.0
%hind 2.0 3.0 6.0 0.4 0.2
%hcap 3.0 3.1 6.0 0.1 0.4
%hres 3.1 4.1 6.0 0.4 0.2
%vres 4.1 4.5 6.0 0.2 0.4
%hcap 4.1 5.0 6.0 0.1 0.4
%vtrn 5.0 5.4 4.5 6.0 0.2 0.4
%vres 6.0 4.5 6.0 0.2 0.4
%term 2.0 4.5 2.0 6.0
;
2.4  6.4 "M[m]"
3.0  6.4 "C[m]"
3.6  6.4 "R[m]"
4.5  6.4 "C[s]"
5.2  6.4 "G[m]"
3.6  5.2 "R[i]"
6.45 5.2 "R[c]"
;
;---------------- 6 parameter middle-ear model --------------------
;
%wire 2.0 1.3 6.0 1.3
%wire 2.0 3.0 2.8 3.0
%wire 2.8 2.6 2.8 3.0
%vcap 2.8 2.4 2.6 0.4 0.1
%vres 2.8 1.3 2.4 0.2 0.4
%hind 2.8 4.6 3.0 0.4 0.2
%vres 4.6 1.3 3.0 0.2 0.4
%hcap 4.6 6.0 3.0 0.1 0.4
%vres 6.0 1.3 3.0 0.2 0.4
%term 2.0 1.3 2.0 3.0
;
3.3  2.4 "C[e]"
3.3  1.7 "R[e]"
3.7  3.4 "M[m]"
5.3  3.4 "C[s]"
5.05 2.0 "R[i]"
6.45 2.0 "R[c]"
