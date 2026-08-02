; fig1.plt - Sample Plot
;
header=no 		; don't put header on plot
xllc=2 			; frame 2 inches from left
yllc=2 			; frame 2 inches from bottom. 
xlen=5 			; frame 5 inches wide 
ylen=4 			; frame 4 inches tall
xmin=0 
xmax=60 
xint=3.2 		; x axis has 3 major & 2 minor intervals
ycycle=2 		; y axis is logarithmic
ymin=10 
ymax=1000 
xlabel=Intensity (dB SPL) 
ylabel=Estimated Loudness 
tlabel=Average of 4 subjects 
pltype=symbols 
symbol=1 		; symbol 1 is an open circle 
data 
10,150,1.5 
20,300,1.25 
50,120,0 
plot 
symbol=10 		; symbol 10 is a filled square
data 
10,50,1.2 
20,80,1.3 
50,45,0.0 
plot 
msgdat=yes 		; place message in data coordinates
mxllc=5 		; lower-left at x=5
myllc=20 		; lower-left at y=20 
"Experimental Conditions: 
|1| F[1] - 1000 
|10| F[2] - 2000" 
! What does this line do?