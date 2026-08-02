; BTNI Computer Systems
; December, 1987
;
;a=-1 : b=-6 ; line weights
a=0.6 : b=1.2 ; line weights

pltlwt=a
sort=0
axlwt=0
annlwt=0
xllc=0
yllc=0
xlen = 11
ylen = 8.5
xmin=0
ymin=0
xmax=11
ymax=8.5

;draw lines to circles
pltype=lines
data
4,4
1.0,6.0
4,4
1.25,6.0
4,4
1.5,6.0
4,4
1.75,6.0
4,4
2.0,6.0
4,4
2.25,6.0
4,4
2.5,6.0
4,4
2.75,6.0
4,4
3.0,6.0
4,4
3.25,6.0
4,4
3.5,6.0
4,4
3.75,6.0
4,4
4.0,6.0
4,4
4.25,6.0
4,4
5.25,6.0
4,4
5.5,6.0
4,4
plot

;ethernet connections
pltlwt=b
4.5,6.0
4,4
4.75,6.0
4,4
5.0,6.0
4,4
3.75,4
3.75,2
plot
4.25,4
4.25,2
plot

; legend
4.0,1.25
4.5,1.25
plot

;other systems
pltlwt=a
1.5,4.75
4,4
1.5,3.5
4,4
1.5,2.25
4,4
3,2.5
4,4
6.25,3.20
4,4
plot

; more legend
1.25,1.25
1.75,1.25
plot

;pdp 11 lines
pltlwt=b
6.25,6.20
4,4
6.25,5.0
4,4
6.25,4.40
4,4
6.25,3.80
4,4
6.25,1.4
plot
pltlwt=a
6.25,2.6
4,4
plot
;
;plot circles
;
pltyp=symb
pltlwt=b
symbol=1
symsiz=-2.
4,4
plot
;other circles
symsiz=-1.
1.5,4.75
1.5,3.5
1.5,2.25
plot
symsiz=-.25
; pc circles
1.5,6.1
1.75,6.1
2.0,6.1
2.25,6.1
2.5,6.1
2.75,6.1
3.0,6.1
3.25,6.1
3.5,6.1
3.75,6.1
4.0,6.1
4.25,6.1
plot
1,6.0
1.25,6.0
1.5,6.0
1.75,6.0
2.0,6.0
2.25,6.0
2.5,6.0
2.75,6.0
3.0,6.0
3.25,6.0
3.5,6.0
3.75,6.0
4.0,6.0
4.25,6.0
4.5,6.0
4.75,6.0
5,6.0
5.25,6.0
5.5,6.0
plot

;pdp-11 circles
symsiz = -0.4
6.25,6.20
6.25,5.60
6.37,5.0  ;palb
6.25,5.0
6.25,4.4
6.25,3.80
6.25,3.2
6.37,2.60  ;hsplb
6.25,2.60
6.25,2.00
6.25,1.4
plot

;imagen square
symbol = 0
symsiz = -0.25
3,2.5
3.75,2
4.25,2
plot

mvalign = bottom
mhalign = left
msgsiz = -0.3
1.55,7.25,"BTNI Computer Systems"

msgsiz = -0.2
3.0,6.90,"December, 1987"

msgsiz = -0.15
2.00,6.30,"IBM-PC Workstations"
2.00,6.30,"___________________"
6.25,6.60,"PDP-11 Laboratory Systems"
6.25,6.60,"_________________________"

mvalign =half
6.80,6.20,"Auditory Physiology"
6.80,5.60,"Electronics Shop A"
6.80,5.00,"Psychoacoustics A, B"
6.80,4.40,"Communication Engineering"
6.80,3.80,"Speech Physiology"
6.80,3.20,"Audiology"
6.80,2.60,"Human Sensory Physiology A, B"
6.80,2.00,"ENG / Vestibular"
6.80,1.40,"Animal Vocalization"
2.00,1.25,"Serial link"
4.75,1.25,"Ethernet"

mhalign =center
3.00,2.25,"Modems"
4.00,1.75,"Imagen Laser Printers"

mvalign =half
1.50,4.75,"MUMPS"
1.50,3.50,"WANG"
1.50,2.25,"RSX"
4.00,4.30,"VAX - 11/750"
4.00,4.00,"UNIX"
4.00,3.70,"4.2 BSD"

