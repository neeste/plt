; butterfly.plt - example of all 12 possible message alignments
;
vxllc=1
vyllc=.1
sort = 0
axlwt = 0
annlwt = 0
header = 0
xllc = 0
yllc = 0
xmin = 0
xmax = 11
ymin = 0
ymax = 8.5
xlen  =  11
ylen  =  8.5
;prml

pltyp=rectangle
0.75,1.25
7.25,6.75
plot

pltyp = symbol
pltlwt = 1.5
symbol = 7
symsiz = -0.125
7,2
7,3
7,4
7,5
7,6
4,2
4,3
4,4
4,5
4,6
1,2
1,3
1,4
1,5
1,6
plot
;prml

msgsiz = -.2
msgdat = yes
msglwt = 1

mhalign = right
mvalign = top
7,1,"right"
mvalign = bottom
7,2,"Butterfly"
mvalign = base
7,3,"Butterfly"
mvalign = half
7,4,"Butterfly"
mvalign = cap
7,5,"Butterfly"
mvalign = top
7,6,"Butterfly"

mhalign = center
mvalign = top
4,1,"center"
mvalign = bottom
4,2,"Butterfly"
mvalign = base
4,3,"Butterfly"
mvalign = half
4,4,"Butterfly"
mvalign = cap
4,5,"Butterfly"
mvalign = top
4,6,"Butterfly"

mhalign = left
mvalign = top
1,1,"left"
mvalign = bottom
1,2,"Butterfly"
mvalign = base
1,3,"Butterfly"
mvalign = half
1,4,"Butterfly"
mvalign = cap
1,5,"Butterfly"
mvalign = top
1,6,"Butterfly"

mhalign = left
mvalign = bottom
7.5,2,"bottom"
mvalign = base
7.5,3,"base"
mvalign = half
7.5,4,"half"
mvalign = cap
7.5,5,"cap"
mvalign = top
7.5,6,"top"

msgsiz = -.25
mhalign = center
mvalign = top
4,0.5,"mhalign"

mhalign = left
mvalign = base
7.5,6.75,"mvalign"

