; fig8.plt - Example 2
;
;   Gap detection for four subjects for apical-basal
;    and mono-bipolar comparisons
;
a=1.7 : b=1.2 : c=0.4 : d=0.9
xllc=a : xlen=4 : xmin=20 : xmax=1000 : xcyc=3 : xper=80
yllc=b : ylen=3 : ymin=1  : ymax=64   : ycyc=6 : yper=80 : yint=-2
head=0 : ticdir=inward : annsiz = 1.5 : yhor=yes
msgsize=2 : mhalign=center
xlen,  6.2, "Cochlear Implant Gap Detection"
msgsize=2.5
xlen, -.7, "Level (^m^A)"
msgangle=90
-.6, ylen, "Gap Threshold (msec)"
msgsize=1.25 : msgangle=0 : mhalign=left
pltype=both  : lintype=0 : symsiz=1 : axlwt=1.5

symbol = 10                ; RS 1000 pps 100 us/phase (1,22)
     50.12    31.3
     63.1     18.6
     79.43    11.8
    100        8.7
    125.89     3.8
    141.25     2.6
plot
symbol = 1                ;  RS  (1,2)  1000 Hz 100 us/phase
    125.89    16.71
    141.25    14.47
    158.49    19.91
    177.83     7.43
    199.53     7.03
    223.87     3.81
    251.19     2.87
plot
c, d, "RS
|10| (1,22)
|1| (1,2)"

newframe
xllc=a      : xanskip = -1
yllc=b+ylen
symbol = 10                ; MM 1000 Hz  100 us/phase  (1,22)
     63.1     32.0
     79.43    15.2
    100       13.6
    125.89     6.0
    158.49     3.4
    199.53     2.0
plot
symbol = 1                ; MM 1000 Hz  100 us/phase  (1,2)
    125.89    14.3
    158.49    13.9
    199.53     7.9
    251.19     5.4
    316.23     2.6
    398.11     1.5
    446.68     1.5
plot
symbol = 2                ; MM 1000 Hz  100 us/phase (20,21)
    125.89    26.8
    158.49    12.3
    199.53     8.1
    251.19     5.6
    316.23     2.6
    398.11     2.2
plot
c, d, "MM
|10| (1,22)
|1| (1,2)
|2| (20,21)"

newframe
xllc = a+xlen : xanskip = 1
yllc = b      : yanskip =-1
symbol = 1                ;  GC  1000 Hz 100 us/phase  ((1,2)
     35.48    47.2
     44.67    38.7
     56.23    36.34
     70.79    11.47
     89.13    12.5
    112.2      6.62
    141.25     5.73
    177.83     3.72
    199.53     3.12
plot
symbol = 2                ;  GC  1000 Hz  100 us/phase  (15,16)
    251.19   26.55
    316.23   19.81
    398.11   10.22
    501.19   11.11
    562.34    8.1
    630.96    3.51
    707.95    2.95
plot
symbol = 10                ;   GC  1000 Hz  100 us/phase   (1,16)
    35.48    21.65
    44.67     5.88
    56.23     7.43
    70.79     3.82
    89.13     4.41
    112.2     3.04
plot
c, d, "GC
|10| (1,16)
|1| (1,2)
|2| (15,16)"

newframe
xllc=a+xlen : xanskip = -1
yllc=b+ylen : yanskip = -1
symbol = 10                ;  BC  (1,22)  1000 pps  100 us/phase
     39.81     40.77
     50.12     15.29
     63.1       5.24
     79.43      5.2625
    100         2.78
plot
symbol = 2                ;   BC  (10,11)  1000 pps 100 us/phase
    316.23    26.34
    398.11    10.83
    501.19     6.25
    630.96     4.94
    794.33     2.7
   1000        1.97
plot
symbol = 1                ;   BC  (1,3)   1000 pps  100 us/phase
    223.87     12.11
    251.19      8.33
    281.84      5.24
    316.23      4.81
    354.81      3.04
    398.11      2.14
    446.68      2.02
plot
c, d, "RC
|10| (1,22)
|1| (1,3)
