;include peb.pal
; define a new color palette
%define RGB  "($1/1000) ($2/1000) ($3/1000)"
%define CMYK "(1-($1+$4)/100) (1-($2+$4)/100) (1-($3+$4)/100)"
data
%RGB      0     0     0 ; black
%RGB      0     0   667 ; blue
%RGB      0   667     0 ; green
%RGB      0   667   667 ; cyan
%RGB    667     0     0 ; red
%RGB      0     0   333 ; midnight blue
%RGB    667   333     0 ; brown
%RGB    667   667   667 ; lt grey
%RGB    333   333   333 ; dk grey
%RGB    333   333  1000 ; lt blue
%RGB    333  1000   333 ; lt green
%RGB    333  1000  1000 ; lt cyan
%RGB   1000   333   333 ; lt red
%RGB   1000   333  1000 ; lt magenta
%RGB   1000  1000   333 ; yellow
%RGB   1000  1000  1000 ; white
palette

;include colorit.txt
fgcol=15 : pltcol=14 : bgcol=5

header=no
; DATA FILE NAME: ab.plt
;
; DATE CREATED:   6-May-92
;
;     20 dB on ordinate = 1 octave on abscissa
;
symfnt audsym.pft

HEADER = NO
XCYCLE =      2.0
XMIN    =   250.
XMAX    =  8000.000
XINT    =     1.
YCYCLE  =     0.
YMIN    =   130.000
YMAX    =     0.000
YINT    =    13.0
XLLC    =     1.00
YLLC    =     0.75
GRID    =     0.
XLENGTH =     4.4505
YLENGTH =     6.0
PLTYPE  =     3.000
TICDIR  =     1.000
MSGSIZ  =     1.400
LABSIZ  =     1.000
ANNSIZ  =     1.000
ANNLWT  =     1.000
LABLWT  =     1.000
PLTLWT  =     1.000
ECHO    =     0.
AXLWT   =     1.000
MSGLWT  =     1.000
YHOR    =     1.000
XANSKP  =     0.
YANSKP  =     0.
XPERCENT=    90.000
YPERCENT=    85.714
ymin=60 : yint=(ymin/10) : yper=100*yint/(yint+2)
XLABEL  =
YLABEL  =HEARING LEVEL (dB HL)
ANNSIZ = 0.9
XANNOT = 250,250 500,500 1000,1K 2000,2K 4000,4K 8000,8K
XFMT    =I4
YFMT    =I4

NEWFRAME
;UNAIDED AUDIOGRAM
XANSKP  =     0.000
YANSKP  =     0.

MHALIGN = CENTER
MSGDAT =      NO
MXLLC   =     4.45
MYLLC   =     -0.5
MSGSIZ  =     1.2
"FREQUENCY (Hz)"
PLOT

MXLLC   =     4.45
MYLLC   =     6.75
MSGSIZ  =     1.6
"Noise Exposure: Firecrackers"
PLOT

MXLLC   =     4.45
MYLLC   =     6.45
MSGSIZ  =     1.3
"(Patient: 62-73-36)"
PLOT

MXLLC   =     2.225
MYLLC   =     6.1
MSGSIZ  =     0.9
"Age:  6 yr - 6 mon"
PLOT

SYMBOL  =     11.00    ; Right Ear
SYMSIZ  =     1.350
LINTYPE =     0.000
DATA
   250.000    0
   500.000    0
  1000.000    0
  2000.000    0
  3000.000    0
  4000.000    40
  6000.000    15
  8000.000    0
PLOT

SYMBOL  =     0        ; Left Ear
SYMSIZ  =     1.350
LINTYPE =     0.000
DATA
   250.000    10
   500.000     0
  1000.000     0
  2000.000     0
  3000.000    20
  4000.000    35
  6000.000    10
  8000.000     0
PLOT

;include spspec.inc        ; Plot mean speech spectrum.

;---------------------------------------------------------
; Long term Average of Speech at 70 dB SPL.
;
;
;      Adapted from Cox, R.  1983  Using ULCL measures to find
;          frequency/gain and SSPL 90
;     Hearing Instruments  34(7), 17-21,39
;
;   Shade area of long term speech spectrum.
;----------------------------------------------------------
pltype = polygon : shade = 7
; Lower limits of long term speech spectrum (Mean - 18 dB).
data
   250.0    15.5
   500.0    35.0
  1000.0    29.5
  1500.0    32.5
  2000.0    24.0
  3000.0    24.5
  4000.0    20.5
;  6000.0    15.5
; Upper limits of long term speech spectrum (Mean + 12 dB).
;  6000.0    45.5
  4000.0    50.5
  3000.0    54.5
  2000.0    54.0
  1500.0    62.5
  1000.0    59.5
   500.0    65.0
   250.0    45.5
; Close the polygon.
   250.0    15.5
plot
pltype = 3


YANSKP  =     -1
YLABEL  =
XLLC    =     5.4505
YLLC    =     0.75
NEWFRAME

SYMBOL  =     11.00    ; Right Ear
SYMSIZ  =     1.350
LINTYPE =     0.000
DATA
   250.000     5
   500.000     10
  1000.000     5
  2000.000     0
  3000.000     0
  4000.000    45
  6000.000     5
  8000.000     0
PLOT

SYMBOL  =     0        ; Left Ear
SYMSIZ  =     1.350
LINTYPE =     0.000
DATA
   250.000     5
   500.000    10
  1000.000     0
  2000.000    -5
  3000.000     0
  4000.000    35
  6000.000     5
  8000.000    10
PLOT

MXLLC   =     2.225
MYLLC   =     6.1
MSGSIZ  =     0.9
"Age:  6 yr - 10 mon"
PLOT

;include spspec.inc        ; Plot mean speech spectrum.
;---------------------------------------------------------
; Long term Average of Speech at 70 dB SPL.
;
;
;      Adapted from Cox, R.  1983  Using ULCL measures to find
;          frequency/gain and SSPL 90
;     Hearing Instruments  34(7), 17-21,39
;
;   Shade area of long term speech spectrum.
;----------------------------------------------------------
pltype = polygon : shade = 7
; Lower limits of long term speech spectrum (Mean - 18 dB).
data
   250.0    15.5
   500.0    35.0
  1000.0    29.5
  1500.0    32.5
  2000.0    24.0
  3000.0    24.5
  4000.0    20.5
;  6000.0    15.5
; Upper limits of long term speech spectrum (Mean + 12 dB).
;  6000.0    45.5
  4000.0    50.5
  3000.0    54.5
  2000.0    54.0
  1500.0    62.5
  1000.0    59.5
   500.0    65.0
   250.0    45.5
; Close the polygon.
   250.0    15.5
plot
pltype = 3

FINISH


