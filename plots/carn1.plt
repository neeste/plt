;  THREE DIFFERENT MEASURES ARE PLOTTED ON THE Y AXIS AND
;  SHARE THE X AXIS (TIME).  FIRST THE PRESSURE, THEN THE
;  Qa, AND, THEN, THE Fo--EACH WITH ITS OWN SET OF Y AXES.
;
;  WE CANNOT INDEPENDENTLY ELIMINATE THE X OR Y AXIS, SO,
;  WE DRAW THE AXES AND TICK MARKS OURSELVES.
;
;
header=no
ytic=.4 : xtic=.5 : ticdir=outward
openax=y : yhor=y
 
xmin = -2 : xmax=20 : xint=11 : xlen=7
ymin = 4 : ymax=8 : yint=4 : ylen=1.5 ;: yllc=yllc+.25
annsiz=1.5

;
;  Po
;
yannot= 5,5 6,6 7,7
xannot=0,0\n1 2,2\n2 4,4 6,6 8,8 10,10\n3 12,12 14,14\n4 16,16 18,18\n5 20,20\n6
msgdat=y : mhalign=right : msgsiz=1.5
-1,3.35,"Weeks
Sessions"
;
; y axis label
;
mhalign=center : msgsiz=1.4
mvalign=half
-4.8,6.4,"P[s]
(cm of
H[2]O)"
symsize=.8 : symbol=11
pltype=both
data
0 6.3 .4
2 5.2 .5
10 6.7 .5
14 5.8 .4
18 6.9 .3
20 5.4 .4
plot
;
; Qa
;
newframe
xaxis=0
ymin = 45 : ymax=70 : yint=5
xanskp=-1
yllc=(yllc)+1.75
yannot=50,50 55,55 60,60 65,65
;
; y axis label
;
-4.8,57.5,"Q[a]"
pltype=both
data
0 56.1 6.3
2 65.8 4.5
10 63.1 3.3
14 50 4.6
18 55.3 4.8
20 62.8 3.8
plot

;
; Fo
;
newframe
ymin = 130 : ymax=190 : yint=6 : ylen=2
xanskp=-1
yllc=(yllc)+1.75
yannot=140,140 150,150 160,160 170,170 180,180
;
; y axis label
;
-4.8,160,"F[0]
(Hz)"
pltype=both
data
0 158.2 9.8
2 167.0 2.6
10 176.2 4.2
14 139.2 6.6
18 164.8 5
20 178.5 6.4
plot


