; Box and Whisker plot
;
; Kanae Nishi
; 5/8/2014
;

header=no
openax = no
ticdir=in
ticsiz = 0.5
annsiz=1.2
labsiz = 1.5
msgdat = yes
ylabel = PERCENT CORRECT



xllc=1.0 : xlen=2.5 : xmin=0 : xmax=4 : xint=0 : xpercent = 98

yllc=1.125 : ylen=6 : ymin=40 : ymax=100 : yint=6
yhor=yes : yanskp=0 : ypercent=90

; =====================================================================


; %define is a macro whose body starts on the line with "%%"

%define box ; <xloc> <min> <10%> <25%> <50%> <75%> <90%> <max> <median>

pltyp=rect : ebnum=1 : ebwid=1.5
w=0.12 ; half-width of box


; whiskers

($1-w) $4 ($4-$3) ; 10 & 25 percentile, negative whisker
($1+w) $5 ; 50 percentile
($1-w) $6 ($7-$6) ; 75 & 90 percentile, postive whisker
($1+w) $5 ; 50 percentile
plot


; median

pltyp=symbol
symbol=11
pltlwt = 0.1
filcol = 0 ; black
symsiz = 1
($1) $9 ; median
plot



pltlwt = 1
; min & max

pltyp=symbol
symbol=1
symsiz = 0.6
($1) $2 ; min
($1) $8 ; max
plot


; macro body

%%
; x-axis location
a1=0.5
a2=a1+1/3
a3=a2+1/3
b1=a1+1+0.2
b2=a2+1+0.2
b3=a3+1+0.2
c1=b1+1+0.2
c2=b2+1+0.2
c3=b3+1+0.2

; x-axis label
xanno = a2,6-7 b2,8-9 c2,Adult


; ##########  0 dB SNR   #################################


;    $1   $2   $3   $4    $5    $6    $7    $8    $9
;   xloc min  10%   25%   50%   75%   90%   max   median 

shade = 21 ; Current study -- 4 vowels
shdcol=7; light grey

%box a1 53.88 56.41 60.91 68.85 72.67 77.67 80.35 69.31 ; 6-7 yr
%box b1 55.00 61.05 62.78 69.92 76.52 77.48 83.33 70.76 ; 8-9 yr
%box c1 67.26 71.67 73.31 78.00 82.91 83.70 90.88 77.60 ; Adult

shdcol =0 ; black  Current study -- /a/ only
shade = 35 ; hatched
%box a2 50.00 63.07 68.18 72.74 77.78 84.92 95.00 71.36 ; 6-7 yr
%box b2 60.00 63.75 75.00 78.30 82.20 88.33 95.00 79.51 ; 8-9 yr
%box c2 66.67 67.00 77.50 84.93 94.06 95.41 100.0 88.19 ; Adult

shade = 21 ; Nishi et al. -- /a/ only
shdcol=15 ; white
%box a3 53.85 69.23 69.23 77.88 84.62 92.31 92.31 76.92 ; 6-7 yr
%box b3 76.92 76.92 84.62 85.58 92.31 92.31 92.31 84.62 ; 8-9 yr
%box c3 61.54 71.54 84.62 88.46 92.31 100.0 100.0 92.31 ; Adult

; Panel title

msgdat = yes : mhalign = center : mvalign = half : msgsiz = 1.2
2.0 105 "0 dB SNR"
plot



; ##########  5 dB SNR   #################################

NewFrame;
xllc=3.5 : xlen=2.5 : xmin=0 : xmax=4 : xint=0 : xpercent = 98

yllc=1.125 : ylen=6 : ymin=40 : ymax=100 : yint=6
yhor=yes : yanskp=-1 : ypercent=90 : ylabel =


msgdat = yes : mhalign = center : mvalign = half : msgsiz = 1.2
2.0 105 "5 dB SNR"
plot


;    $1   $2   $3   $4    $5    $6    $7    $8    $9
;   xloc min  10%   25%   50%   75%   90%   max   median 

shade = 21 ; Current study -- 4 vowels
shdcol=7; light grey

%box a1 73.21 80.89 83.14 85.91 89.60 92.01 93.53 86.43 ; 6-7 yr
%box b1 77.90 86.84 87.45 88.95 92.08 93.08 93.75 89.24 ; 8-9 yr
%box c1 84.72 86.07 90.92 91.86 94.02 95.39 96.27 92.86 ; Adult

shdcol =0 ; black  Current study -- /a/ only
shade = 35 ; hatched
%box a2 68.18 83.64 87.22 89.45 93.92 99.44 100.0 90.00 ; 6-7 yr
%box b2 81.25 88.89 88.89 91.72 95.00 99.50 100.0 90.91 ; 8-9 yr
%box c2 83.33 89.38 94.27 95.07 100.0 100.0 100.0 95.00 ; Adult

shade = 21 ; Nishi et al. -- /a/ only
shdcol=15 ; white
%box a3 76.92 76.92 82.69 88.78 92.31 100.0 100.0 92.31 ; 6-7 yr
%box b3 76.92 79.23 90.38 91.03 92.31 100.0 100.0 92.31 ; 8-9 yr
%box c3 61.54 79.23 84.62 91.35 100.0 100.0 100.0 92.31 ; Adult



; #################### 10 dB SNR ################################

NewFrame;
xllc=6.0 : xlen=2.5 : xmin=0 : xmax=4 : xint=0 : xpercent = 98

yllc=1.125 : ylen=6 : ymin=40 : ymax=100 : yint=6
yhor=yes : yanskp=-1 : ypercent=90 : ylabel =


msgdat = yes : mhalign = center : mvalign = half : msgsiz = 1.2
2.0 105 "10 dB SNR"
plot


;   $1   $2   $3   $4   $5  $6   $7   $8   $9
;  xloc min  10%  25%  50%  75%  90%  max  median

; Current study all 4 vowels
shade = 21 ; solid col
shdcol=7; light grey
%box a1 89.06 89.50 90.21 94.07 96.25 98.14 100.0 94.66 ; 6-7 yr
%box b1 89.06 91.75 93.44 95.01 96.66 97.64 100.0 95.64 ; 8-9 yr
%box c1 93.06 95.05 96.19 97.15 98.52 99.88 100.0 96.85 ; Adult

; Current study /a/ only
shdcol =0 ; black
shade = 35 ; hatched
%box a2 81.82 93.75 94.27 96.18 100.0 100.0 100.0 97.73 ; 6-7 yr
%box b2 87.50 90.38 94.27 95.09 96.59 100.0 100.0 95.00 ; 8-9 yr
%box c2 88.89 100.0 100.0 99.07 100.0 100.0 100.0 100.0 ; Adult

; Nishi et al /a/ only
shade = 21
shdcol=15 ; white
%box a3 69.23 84.62 84.62 91.35 94.23 100.0 100.0 92.31 ; 6-7 yr
%box b3 76.92 92.31 92.31 94.55 100.0 100.0 100.0 92.31 ; 8-9 yr
%box c3 69.23 92.31 92.31 96.47 100.0 100.0 100.0 100.0 ; Adult
plot


; ========   Legend inside the main frame  ===============

k=1.0
d= 40
e= 42 
f= 44  
g= 46
h= 50 
i= 52
j= 54
l= 48

;   $1   $2    $3    $4   $5    $6    $7    $8   $9
;  xloc min   10%   25%   50%   75%   90%   max  median

%box k d e f g h i j l ; KEY

m=k+0.3
msgdat=yes : mhalign=left : mvalign=half : msgsiz=0.9

(m) j "maximum"
(m) i "90th percentile"
(m) h "75th percentile"
(m) g "mean"
(m) f "25th percentile"
(m) e "10th percentile"
(m) d "minimum"
(m) l "median"


%undefine box ; undo macro


pltype = 5 ; rectangle
shade = 0 ; no fill

; frame
; x  y

0.2 65
3.8 38
plot

; horizontal line

pltype = 2 ; line
0.2 56
3.8 56
plot


mhalign = center
MSGSIZE = 0.9
1.7, 63.5 "Present" 
1.7, 62 "study"

3.0 63.5 "Nishi et al."
3.0 62 "(2010)"
shdcol = 0;

MSGSIZE = 0.9
0.7, 57.5 "4 Vs"
0.7, 60 "/a/"

MSGSIZE = 1.0
shdcol = 0;
1.7 60 "|#36|"

MSGSIZE = 1.0
shdcol = 7;
1.7 57.5  "|#21|"



shdcol = 0;
3.0 60  "|#20|"

plot



finish