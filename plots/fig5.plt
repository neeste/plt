; fig5.plt - example of all 12 possible message alignments
;
sort = 0
axlwt = 0
annlwt = 0
header = 0
xllc = 1.25
yllc = 1
xmin = 0
xmax = 11
ymin = 0
ymax = 8.5
xlen  =  7
ylen  =  7

pltyp=rectangle
0.75,1.25
7.25,6.75
plot

pltyp = symbol
pltlwt = 1.5
symbol = 7
symsiz = 1
7.0,2
7.0,3
7.0,4
7.0,5
7.0,6
4.0,2
4.0,3
4.0,4
4.0,5
4.0,6
1.0,2
1.0,3
1.0,4
1.0,5
1.0,6
plot

msgsiz = 0.8
msgdat = yes
msglwt = 1

mxllc = 7.0
mhalign = right
myllc = 1
mvalign = top
"right"
myllc = 2
mvalign = bottom
"Butterfly"
myllc = 3
mvalign = base
"Butterfly"
myllc = 4
mvalign = half
"Butterfly"
myllc = 5
mvalign = cap
"Butterfly"
myllc = 6
mvalign = top
"Butterfly"

mxllc = 4.0
mhalign = center
myllc = 1
mvalign = top
"center"
myllc = 2
mvalign = bottom
"Butterfly"
myllc = 3
mvalign = base
"Butterfly"
myllc = 4
mvalign = half
"Butterfly"
myllc = 5
mvalign = cap
"Butterfly"
myllc = 6
mvalign = top
"Butterfly"

mxllc = 1.0
mhalign = left
myllc = 1
mvalign = top
"left"
myllc = 2
mvalign = bottom
"Butterfly"
myllc = 3
mvalign = base
"Butterfly"
myllc = 4
mvalign = half
"Butterfly"
myllc = 5
mvalign = cap
"Butterfly"
myllc = 6
mvalign = top
"Butterfly"

mxllc = 7.5
mhalign = left
myllc = 2
mvalign = bottom
"bottom"
myllc = 3
mvalign = base
"base"
myllc = 4
mvalign = half
"half"
myllc = 5
mvalign = cap
"cap"
myllc = 6
mvalign = top
"top"

msgsiz = 1
mxllc = 4.0
myllc = 0.5
mhalign = center
mvalign = top
"mhalign"

mxllc = 7.5
myllc = 6.75
mhalign = left
mvalign = base
"mvalign"
