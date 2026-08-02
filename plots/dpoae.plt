MAGNIFY=1.5
HEADER =    NO
GRID =     YES
GRDLWT =   1.5
GRDTYPE =    2
YHOR =     YES
XPERCENT =  85
YPERCENT =  85
TICDIR =     2
EBNUM =      0
XLEN =       3
YLEN =       3
XMIN =       1
XMAX =       8
XINT =       7
XANNOT = 1,.75 2,1 3,1.5 4,2 5,3 6,4 7,6 8,8
LABSIZ =   1.5
ANNSIZ =  1.25
MSGDAT =   YES

;---------------------- DPOAE ----------------------
NEWFRAME
YMIN =   -30
YMAX =    20
YINT =    10
XANSKP  =  0
YANSKP  =  0
XLLC =     1
YLLC =     1
YLABEL = DPOAE Level (dB SPL)
XLABEL = f[2] Frequency (kHz)

;1.25        16     "DPOAE"
PLOT

PLTYPE = 2
LINTYPE = 0;          95TH PERCENTILE
1      5.95
2      7.76
3      3.84
4     -0.93
5     -2.50
6      0.18
7     -2.13
8     -9.96
PLOT

LINTYPE = 0;          5TH PERCENTILE
1    -13.60
2    -12.35
3     -9.80
4    -13.87
5    -16.25
6     -9.55
7    -11.05
8	-20
PLOT

LINTYPE = 10;          90TH PERCENTILE
1      2.41
2      4.40
3      0.43
4     -3.51
5     -5.55
6     -4.43
7     -6.87
8    -12.84
PLOT

LINTYPE = 10;          10TH PERCENTILE
1    -10.40
2     -8.12
3     -6.83
4     -9.82
5    -11.50
6     -5.93
7     -7.83
8	-20
PLOT

PLTYPE = 6
SHADE = 16;          95TH TO 5TH PERCENTILE SHADING
1      5.95
2      7.76
3      3.84
4     -0.93
5     -2.50
6      0.18
7     -2.13
8     -9.96
8	-20
7    -11.05
6     -9.55
5    -16.25
4    -13.87
3     -9.80
2    -12.35
1    -13.60
PLOT

SHADE = 40;          90TH TO 10TH PERCENTILE SHADING
SHDGRAY = .75
1      2.41
2      4.40
3      0.43
4     -3.51
5     -5.55
6     -4.43
7     -6.87
8    -12.84
8	-20
7     -7.83
6     -5.93
5    -11.50
4     -9.82
3     -6.83
2     -8.12
1    -10.40
PLOT