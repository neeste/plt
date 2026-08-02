; ABR Wave V latency - 5 msec vs. frequency
;
wxlen=8 : clip=yes : head=no
msgsiz=-.13 : labsiz=-.18 : annsiz=-.15 : symsiz=1
xllc=1.75 : xcyc=2 : xmin=250 : xmax=8000 : xint=-2 : xlen=5 : xper=80
yllc=1.75 : ycyc=1 : ymin=1   : ymax=16   : yint=0  : ylen=5 : yper=100
xlabel = frequency (Hz)
ylabel = latency (msec)
yhor=yes : ticdir=in
yannot= 1 2 4 8 16

pltyp=lines : lintyp=0
; line fits to data at each intensity
; latency = 13 * 500**(-i/100.) * f**(-0.41)
;      f = 0.20, 800 (kHz)
 200   13.211
8000    2.911
plot
 200   11.247
8000    2.479
plot
 200    9.575
8000    2.110
plot
 200    8.152
8000    1.796
plot
 200    6.940
8000    1.529
plot

pltype=1 : lintype=4

symbol = 1
   500  9.77
   750  7.65
  1000  6.74
  1500  5.69
  2000  5.13
  3000  4.47
  4000  3.46
  6000  3.23
  8000  2.94
plot

symbol = 3
   250  9.57
   500  8.14
   750  6.77
  1000  5.75
  1500  4.85
  2000  4.36
  3000  3.83
  4000  3.02
  6000  2.70
  8000  2.45
plot

symbol = 4
   250  7.91
   500  6.64
   750  5.69
  1000  4.51
  1500  4.04
  2000  3.83
  3000  3.35
  4000  2.62
  6000  2.31
  8000  2.12
plot

symbol = 6
   250  7.04
   500  5.74
   750  4.56
  1000  3.68
  1500  3.36
  2000  3.22
  3000  2.79
  4000  2.19
  6000  1.94
  8000  1.82
plot

symbol = 5
   250  6.33
   500  5.17
   750  3.67
  1000  2.93
  1500  2.80
  2000  2.69
  3000  2.40
  4000  1.76
  6000  1.63
  8000  1.58
plot

msgsiz=-.13
0.6 1.5 "
|1|   40
|3|   50
|4|   60
|6|   70
|5|   80 dB SPL
"
