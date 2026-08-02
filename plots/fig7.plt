; fig7.plt - Example 1
;
; ABR Wave V latency - 5 msec vs. frequency
;
wxlen=8 : clip=yes : head=no
msgsiz=-.13 : labsiz=-.18 : annsiz=-.15 : symsiz=1
xllc=1.75 : xcyc=2 : xmin=0.25 : xmax=8  : xint=-2 : xlen=5 : xper=80
yllc=2.75 : ycyc=1 : ymin=1    : ymax=16 : yint=0  : ylen=3 : yper=100
xlabel = frequency (kHz)
ylabel = latency (msec)
yhor=yes : ticdir=in
rmin=1 : rmax=16 : rcyc=4 : rint=-2
xgain=0.001
xannot= 0.25 0.5 1 2 4 8
yannot= 1 2 4 10 16

pltyp=lines : lintyp=0
; latency = 13 * 500**(-i/100.) * f**(-0.41), f = 0.20, 800 (kHz)
 200   13.211
8000    2.911
plot
 200    9.575
8000    2.110
plot
 200    6.940
8000    1.529
plot

pltype=1 : lintype=4
symbol = 1                ; 40 dB
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

symbol = 4                ; 60 dB
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

symbol = 5                ; 80 dB
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
0.6 1.25 "
|1|   40
|4|   60
|5|   80 dB SPL
"
