; 
; some constants
;
p=3.14159265359  ; pi
q=6.28318530718  ; 2*pi
;
; polar to Cartesian 
;
%define xval "r*cos($1)+a"
%define yval "r*sin($1)+b"
;
; degrees to radians
;
%define degtorad "($1/360)*q"
;
;  resolution and radian increment
;
t=100
u=q/t
;
;  draw a sector as a polygon
;  start v at a known angle
;  it will increase by the number of degrees in the sector
;
v=180
%define sector   ; <proportion> <shade num>
f=$1*q
g=$1*t
h=degtorad(v)  
pltype=polygon
shade=$2
0 0
(xval(h)) (yval(h))
%repeat g
(xval($$0*u+h)) (yval($$0*u+h))
%%%
(xval(f+h)) (yval(f+h))
0 0
plot
v=v+($1*360)           ; increase angle
%%

;
; NO LABELS, ANNOTATIONS, AXES OR HEADER
;
lablwt=0 : annlwt=0.0 : axlwt=0.0 : header=no
msgdat=y
yllc=1 : xllc=2.75
ylen=5 : xlen=5
j=1.5
xmin=-j : xmax=j : xfmt=f3.1 : xint=8
ymin=-j : ymax=j : yfmt=f3.1 : yint=8 : yhor=y
;
; pie chart is a circle
; radius=1.5;  center at (0,0)
;
r=1.5 : a=0 : b=0
%sector .18 0       ; start at 180 degrees
%sector .07 0
%sector .07 0
%sector .11 0
%sector .57 0
%sector .25 36
;
; add title, labels, etc.
;
msgsiz=1.5 : mhalign=center : mvalign=half
0,2.1,"HARTLEY FAMILY CENTER
Additional Deafness Within Family
1991-1992"

msgsiz=.9
0,.75,"57%
None"
.7,-.65,"11%
oral/HOH"
.27,-1.1,"7%
Unilateral
Loss"
msgsiz=1.1
-2,-.3,"|#36|
DEAFNESS 25%"
msgsiz=.9
-1.65,-.9,"18%
Immediate Family"
-.55,-1.55,"7%
Extended Family"
;
;  pointer lines
;
pltype=lines
-1.5 -.9
-1 -.75
plot
-.4 -1.5
-.3 -1.3
plot
finish


