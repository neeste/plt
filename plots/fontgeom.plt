rotate =yes
vyllc = 1.3
vxllc = -1
axlwt = 0
annlwt=0
header=0
xllc=0
yllc=0
xmin=0
xmax=5.5
ymin=0
ymax=4.0
xlen = 5.5
ylen = 4
pltlwt=-1
;draw square
pltype=lines
lintyp=0
0,0
0,4
5.5,4
5.5,0
0,0
plot
;inside solid lines
0.5,1.0
4.0,1.0
plot
0.5,2.00
4.0,2.00
plot
0.5,3.00
4.0,3.00
plot
1.36,3.5
1.36,.34
plot
3.08,3.5
3.08,.34
plot
;inside dashed lines
lintype = 4
4.,.34
.5,.34
.5,3.44
4.,3.44
plot
2.22,0.5
2.22,3.44
plot
3.94,.34
3.94,3.44
plot
msgsiz = 1
mhalign =left
mvalign =base
mxllc = .34
myllc = 3.7
"left
line"
mxllc = 1.36
myllc = 3.7
"center
line"
mxllc = 2.22
myllc = 3.7
"right
line"
mhalign =left
mvalign =half
mxllc = 4.2
myllc = 3.5
"top line"
myllc = 3
"cap line"
myllc = 2
"half line"
myllc = 1
"base line"
myllc = 0.34
"bottom line"
plot
mhalign =left
mvalign =top
mxllc = 2.22
myllc = 0.34
"left"
mxllc = 3.08
"center"
mxllc = 3.94
"right"
plot
msgangle=0
mhalign =left
mvalign =base  
msglwt = -9 
msgsiz = -2
myllc = 1
mxllc = .5
"Kp"
plot
