magn=1.1 : vxllc=-.3 : vyllc=-.15
head=no  : xllc=.01 : yllc=.01 : xmin=0.0  : xmax=10.5 : ymin=0 : ymax=8
rota=yes : msgd=no  : xint=1.0 : yint=1.0  : xans=-1.0 : wxle=8 : wyle=10.5
mhal=c   : mval=h   : msgs=.34 : xlen=10.5 : ylen=8.0

3.75,10.1,"AUDIOLOGICAL RECORD"
msla=15
3.75,9.97,"BOYS TOWN NATIONAL INSTITUTE
FOR COMMUNICATION DISORDERS IN CHILDREN"
msla=0 : msgs=.28 : mhal=l

msgs=.34
.25,9.65,"    NAME _______________________________ AGE _____________ BOOTH __________________"
msgs=.25 : msla=15
1.3,9.5,"(Last)                      (First)"
msgs=.34 : msla=0
.25,9.15,"    PT # _________________ DATE _________ TESTED BY ________________________________"

;
;                          Audiogram Template
;
; INSTRUCTIONS:  Copy the template file to a new file.  Search for "data"
;                statements.  Insert lines of data in terms of frequency
;                and dB HL immediately after the data statement.
;
;                To fill in the effective masking levels, search for " "
;                and enter values between the quotation marks.  Remember
;                not to leave any spaces between the quotation marks or
;                else the values will not be centered correctly.
;
;==========================================================================
;section 1
;
;              grid1.plt - file used to plot the grid 
;                          on the audiogram template
;
;==========================================================================
rota=1 : ticd=in : yhor=y : tics=0 : msgs=1.0
gaps=4 : syms=1.5 : msgd=y : mhal=c  : mval=b : head=no
;
xllc=1.0 : xlen=3. : xmin=88.388 : xmax=11314 
xint=-2.0 : xans=-1 : xcyc=2.000
;
yllc=5.45 : ylen=3. : ymin=120.00 : ymax=-10.0 : yint=13.1
ylab=HEARING LEVEL (HL) IN dB
plot
;
symfnt audsym.pft
;
1000,-25,"FREQUENCY IN HERTZ (Hz)"
125.,-15,"125"
250.,-15,"250"
500.,-15,"500"
1000,-15,"1000"
2000,-15,"2000"
4000,-15,"4000"          ; Top axis annotations on audiogram
8000,-15,"8000"
msgs=.7
750.,-11,"750"
1500,-11,"1500"
3000,-11,"3000"
6000,-11,"6000"

plty=b : syms=1.5
;
;
;                   Left Ear Response
;
symb=0  ;  Left Ear Air Conduction-Earphones Unmasked
data
plot
symb=1  ;  Left Ear Air Conduction-Earphones Masked
data
plot
plty=s
symb=2  ;  Left Ear Bone Conduction-Mastoid Unmasked
data
plot
symb=3  ;  Left Ear Bone Conduction-Mastoid Masked
data
plot
symb=4  ;  Left Ear Bone Conduction-Forehead Masked
data
plot
symb=5  ;  Left Ear Air Conduction-Sound Field
data
plot
symb=6  ;  Left Ear Acoustic-Reflex Threshold Contralateral
data
plot
symb=7  ;  Left Ear Acoustic-Reflex Threshold Ipsilateral
data
plot
;
;
;                Unspecified Ear Response
;
symb=8  ;  Bone Conduction-Mastoid Unmasked
data
plot
symb=9  ;  Bone Conduction-Forehead Unmasked
data
plot
plty=b
symb=10 ;  Air Conduction-Sound Field
data
plot
;
;
;                  Right Ear Response
;
symb=11 ;  Right Ear Air Conduction-Earphones Unmasked
data
plot
symb=12 ;  Right Ear Air Conduction-Earphones Masked
data
plot
plty=s
symb=13 ;  Right Ear Bone Conduction-Mastoid Unmasked
data
plot
symb=14 ;  Right Ear Bone Conduction-Mastoid Masked
data
plot
symb=15 ;  Right Ear Bone Conduction-Forehead Masked
data
plot
symb=16 ;  Right Ear Air Conduction-Sound Field
data
plot
symb=17 ;  Right Ear Acoustic-Reflex Threshold Contralateral
data
plot
symb=18 ;  Right Ear Acoustic-Reflex Threshold Ipsilateral
data
plot

;
;              grid2.plt - file used to plot the grid 
;                          on the audiogram and the "Effective Masking
;                          Levels" template
;
;==========================================================================

lint=0 : plty=l
;
data
 125,-10
 125,120
 250,120
 250,-10
 500,-10
 500,120
1000,120
1000,-10                 ; Vertical grid lines on audiogram
2000,-10
2000,120
4000,120
4000,-10
8000,-10
8000,120
plot

data
   88,110
11300,110
11300,100
   88,100
   88,90
11300,90
11300,80
   88,80
   88,70
11300,70
11300,60                 ; Horizontal grid lines on audiogram
   88,60
   88,50
11300,50
11300,40
   88,40
   88,30
11300,30
11300,20
   88,20
   88,10
11300,10
plot

pltl=2
data
   89,0                  ; Heavy line at 0 dB HL
11300,0
plot

syms=.75 : pltl=.25 : msgs=.75 : msgl=.75 : mval=h
750,-5,"|124|"
750,5.,"|124|"
750,15,"|124|"
750,25,"|124|"
750,35,"|124|"
750,45,"|124|"
750,55,"|124|"           ; Vertical hashmarks   -   750 Hz
750,65,"|124|"
750,75,"|124|"
750,85,"|124|"
750,95,"|124|"
750,105,"|124|"
750,115,"|124|"
plot

1500,-5,"|124|"
1500,15,"|124|"
1500,25,"|124|"
1500,35,"|124|"
1500,45,"|124|"
1500,55,"|124|"
1500,65,"|124|"
1500,5.,"|124|"          ; Vertical hashmarks   -   1500 Hz
1500,75,"|124|"
1500,85,"|124|"
1500,95,"|124|"
1500,105,"|124|"
1500,115,"|124|"
plot

3000,-5,"|124|"
3000,5.,"|124|"
3000,15,"|124|"
3000,25,"|124|"
3000,35,"|124|"
3000,45,"|124|"
3000,55,"|124|"          ; Vertical hashmarks   -   3000 Hz
3000,65,"|124|"
3000,75,"|124|"
3000,85,"|124|"
3000,95,"|124|"
3000,105,"|124|"
3000,115,"|124|"
plot

6000,-5,"|124|"
6000,5.,"|124|"
6000,15,"|124|"
6000,25,"|124|"
6000,35,"|124|"
6000,45,"|124|"
6000,55,"|124|"          ; Vertical hashmarks   -   6000 Hz
6000,65,"|124|"
6000,75,"|124|"
6000,85,"|124|"
6000,95,"|124|"
6000,105,"|124|"
6000,115,"|124|"
plot

newframe

;section 2

xmin=-1. : xmax=10. : xint=1.00 : xcyc=0
;
ymin=0.0 : ymax=10.0 : yint=1.00 : ylab=   
ylen=.75 : yllc=4.20 : yans=-1.0
;
pltl=.50 : msgl=1.2 : syms=1.25 : plty=l
data
0,0
0,10
1,10
1,0
2,0
2,10
3,10
3,0                      ; "Effective Masking Levels" frame
4,0
4,10                     ; Vertical grid lines
5,10
5,0
6,0
6,10
7,10
7,0
8,0
8,10
9,10
9,0
plot

data
-1,2.5
10,2.5                   ; Horizontal grid lines
10,7.5
-1,7.5
plot

pltl=1.3
data
-1,5                     ; Heavy line between AC and BC
10,5
plot

msgs=1 : pltl=1.0
-.5,11,"125"
0.5,11,"250"
1.5,11,"500"
2.5,11,"750"
3.5,11,"1000"
4.5,11,"1500"            ; Top axis annotations
5.5,11,"2000"
6.5,11,"3000"
7.5,11,"4000"
8.5,11,"6000"
9.5,11,"8000"
plot

msgs=1.5 
;-1.75,13.5,"Non-Test
;Ear"
4.5,13,"EFFECTIVE MASKING LEVELS TO NON-TEST EAR"
-1.55,8.75,"L"           ; Labels for "Effective Masking Levels" frame
-1.55,6.25,"R"
-1.55,3.75,"L"
-1.55,1.25,"R"
msgs=1.75
-2.55,7.5,"AC"
-2.55,2.5,"BC"
plot
msga=90 : msgs=2.0
-3.55,5,"TEST"

msga=0 : plty=r : shad=16
data
-1,0
0,5
plot
shad=17
data                     ; Shaded portion of frame
8,0
10,5
plot
plty=l
;data
;-2.4,10.3
;-1.4,10.3
;plot

msgs=1.3

;
;   Enter effective masking level for non-test ear by typing
;   the actual value between the double quotation marks.
;
;             Air Conduction - Left Ear
;
-.5,8.75," "    ; AC - L   125 Hz
0.5,8.75," "    ; AC - L   250 Hz
1.5,8.75," "    ; AC - L   500 Hz
2.5,8.75," "    ; AC - L   750 Hz
3.5,8.75," "    ; AC - L  1000 Hz
4.5,8.75," "    ; AC - L  1500 Hz
5.5,8.75," "    ; AC - L  2000 Hz
6.5,8.75," "    ; AC - L  3000 Hz
7.5,8.75," "    ; AC - L  4000 Hz
8.5,8.75," "    ; AC - L  6000 Hz
9.5,8.75," "    ; AC - L  8000 Hz
plot

;
;             Air Conduction - Right Ear
;
-.5,6.25," "    ; AC - R   125 Hz
0.5,6.25," "    ; AC - R   250 Hz
1.5,6.25," "    ; AC - R   500 Hz
2.5,6.25," "    ; AC - R   750 Hz
3.5,6.25," "    ; AC - R  1000 Hz
4.5,6.25," "    ; AC - R  1500 Hz
5.5,6.25," "    ; AC - R  2000 Hz
6.5,6.25," "    ; AC - R  3000 Hz
7.5,6.25," "    ; AC - R  4000 Hz
8.5,6.25," "    ; AC - R  6000 Hz
9.5,6.25," "    ; AC - R  8000 Hz
plot

;
;             Bone Conduction - Left Ear
;
0.5,3.75," "    ; BC - L   250 Hz
1.5,3.75," "    ; BC - L   500 Hz
2.5,3.75," "    ; BC - L   750 Hz
3.5,3.75," "    ; BC - L  1000 Hz
4.5,3.75," "    ; BC - L  1500 Hz
5.5,3.75," "    ; BC - L  2000 Hz
6.5,3.75," "    ; BC - L  3000 Hz
7.5,3.75," "    ; BC - L  4000 Hz
plot

;
;             Bone Conduction - Right Ear
;
0.5,1.25," "    ; BC - R   250 Hz
1.5,1.25," "    ; BC - R   500 Hz
2.5,1.25," "    ; BC - R   750 Hz
3.5,1.25," "    ; BC - R  1000 Hz
4.5,1.25," "    ; BC - R  1500 Hz
5.5,1.25," "    ; BC - R  2000 Hz
6.5,1.25," "    ; BC - R  3000 Hz
7.5,1.25," "    ; BC - R  4000 Hz
plot

newframe

;Section 3 "MLV "

xllc=1.0 : yllc=3.0  : xans=-1 : yans=-1.0 : xmin=0 : xmax=3.875 : xint=1.0
ymin=-.5 : ymax=1.25 : yint=1.0 : ylen=1

data
0,0
3.875,0
plot
data
0,.5
3.875,.5
plot
data
0,1.
3.875,1.
plot
data
.40625,-.5
.40625,1.25
plot
data
.8125,-.5
.8125,1.25
plot
data
.875,-.5
.875,1.25
plot
data
1.375,-.5
1.375,1.25
plot
data
1.875,-.5
1.875,1.25
plot
data
2.375,-.5
2.375,1.25
plot
data
2.875,-.5
2.875,1.25
plot
data
3.375,-.5
3.375,1.25
plot
; 1st row diagonals
data
2.094,1
2.375,1.25
plot
data
2.594,1
2.875,1.25
plot
data
3.094,1
3.375,1.25
plot
; 2nd row diagonals
data
1.875,.5
2.375,1
plot
data
2.375,.5
2.875,1
plot
data
2.875,.5
3.3785,1
plot
; 3rd row diagonals
data
1.875,0
2.375,.5
plot
data
2.375,0
2.875,.5
plot
data
2.875,0
3.3785,.5
plot
; 4th row diagonals
data
1.875,-.5
2.375,0
plot
data
2.375,-.5
2.875,0
plot
data
2.875,-.5
3.3785,0
plot
data
0,-.5
.40625,0
plot

msgs=.8 : mhal=c : mval=h
.2,1.125,"EAR"
.62,1.17,"P/T AV.
500-2000"
1.125,1.125,"SRT"
1.625,1.125,"SAT"
2.1,1.1775,"Level"
2.6,1.1775,"Level"
3.1,1.1775,"Level"
2.1,1.0625,"         %"
2.6,1.0625,"         %"
3.1,1.0625,"         %"
3.625,1.18,"Speech
Materials"
plot

msgs=1.2 : mhke=2 : mvke=2
-.4,.75,"MLV |#20|"
-.4,.25,"TAPE |#20|"
plot

.2,.75,"R"
.2,.25,"L"
.12,-.125,"SF"
.3,-.375,"BC"

data
-1,-.875
9.08,-.875
plot

msgs=1.6 : mhal=l
-1,-1.125,"AUDIOLOGIC IMPRESSION:





RECOMMENDATIONS:      "

newframe

;section 4

xllc=4.25 : yllc=8.125 : xans=-1 : yans=-1 : xmin=0 : xmax=1 : xint=1.0
ymin=0 : ymax=1.0 : yint=1.0 : xlen=1. : ylen=.5 : mhal=c : axlw=1.3

msgs=3.6
1.5,1.375,"KEY TO AUDIOGRAM"

data
.5,0
.5,1
plot
data
.75,0
.75,1
plot
data
0,.75
1,.75
plot
data
0,.5
1,.5
plot
data
0,.25
1,.25
plot

mhal=l : mval=h

.15,.875,"EAR"
.15,.625,"AC"
.15,.375,"BC"
.15,.125,"NR"
plot

mhal=c

.625,.875,"R"
.875,.875,"L"

plty=s : syms=3.5
symb=11
data
.625,.625
plot

symb=14
data
.65,.375
plot

symb=28 : syms=4.5
data
.65,.2
plot

symb=0 : syms=3.5
data
.875,.625
plot

symb=3
data
.85,.375
plot

symb=21 : syms=4.5
data
.85,.2
plot

syms=3.5 : plty=l

newframe

;section 5

xllc=4.25 : yllc=7.5 : xans=-1 : yans=-1 : xmin=0 : xmax=1 : xint=1.0
ymin=0 : ymax=1 : yint=1 : xlen=1 : ylen=.5 : msgs=3.4 : mhal=l

.05,.85,"Test Reliability:"
.05,.45,"Method:"
plot

newframe

;section 6   

xllc=5.5 : yllc=7.5 : xans=-1 : yans=-1 : xmin=0 : xmax=1 : xint=1.0
ymin=0 : ymax=1.0 : yint=1.0 : xlen=1.75 : ylen=1.125 : msgs=1.8
mhal=l : mval=t : mhke=1.25 : mvke=1.25 : mvsp=2.5 : axlw=1.0

;.07,.9,"|8|"
plot

msga=90
.07,.9,"\]"
plot

msga=0
msgs=1.5
.07,.94,"
|#27|
SAT
SRT
SL 
SF
BC
+"
plot
.1,.94,"    -  Unmasked BC
    -  Sound Field Warble Tones
    -  Speech Awareness Thr.
    -  Speech Reception Thr.
    -  Sensation Level
    -  Sound Field
    -  Bone Conduction
    -  Single Responses"
plot

newframe

;Section 7 "Equivalent volume"

xllc=4.25 : yllc=6.325 : xans=-1 : yans=-1 : xmin=0 : xmax=3.875 : xint=1.0
ymin=0 : ymax=1.25 : yint=1. : xlen=3. : ylen=1 : mhal=c : mval=h 

data
0,1.
3.875,1.
plot

msgs=1.5	
1.9375,1.125,"EQUIVALENT VOLUME"

msgs=1.3
.5,.8,"      R _____cc
      L _____cc"


2.5,.8,"Release @ _____ da Pa
Release @ _____ da Pa"
plot

newframe

;Section 8 "Special tests"

xllc=4.25 : yllc=5.325 : xans=-1 : yans=-1 : xmin=0 : xmax=3.875 : xint=1.0
ymin=0 : ymax=1.25 : yint=1. : xlen=3. : ylen=1 : mvsp=2

data
0,1.
3.875,1.
plot

msgs=1.5	
1.9375,1.125,"SPECIAL TESTS"

newframe

;Section 9 "Comments"

xllc=4.25 : yllc=3.0 : xans=-1 : yans=-1 : xmin=0 : xmax=3.875 : xint=1.0
ymin=0 : ymax=1.25 : yint=1.0 : xlen=3. : ylen=2.125 : axlwt=.8

data
0,1.125
3.875,1.125
plot

msgs=1.3 : msgl=1.0
1.9375,1.1875,"COMMENTS"
plot

newframe

;Section 10 "Audiological Restest"

xllc=6 : yllc=.75 : xans=-1 : yans=-1.0 : xmin=0.0 : xmax=1 : xint=1.0
ymin=0 : ymax=.50 : yint=1. : xlen=1.25 : ylen=.75 : axlw=1.4

data
0,.4
1,.4
plot

msgs=2 : msgl=1.2
.5,.45,"AUDIOLOGICAL RETEST"
plot


