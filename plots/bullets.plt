;-------------------------->>> Begin <<<-------------------------------
;
;                     PLT File Name: bullets.plt
;
;        (Example of a plt file to make a slide with 'bullets'.)
;
;         Name:    W.J. Kelly           Date: 15-Feb-89
;
;-------------->>>  Description of Frame and Axes  <<<-----------------
;
xllc = 1.5
yllc = 1.5
stdfnt cmprom
lablwt  = 0.0
annlwt  = 0.0
axlwt   = 0.0
symsiz = 0.5
header  = NO
;--------------------->>>  Title of Slide  <<<--------------------------
;
mhalign = CENTER  : msgsiz  =  2.0   : mxllc   =  3.5  : myllc   = 5.2
"TITLE OF SLIDE"
plot
;---------------------------------------------------
;              >>>  Text. <<<
;
mhalign = left    : msgsiz  =  1.5   : mxllc   =  0.0  : myllc   = 3.5
mvkey   = 0.5
"
|11|   Point 1 appears on this line.

|11|   This is point number 2. It
|31|   continues on this line and even
|31|   runs on to this line.

|11|   This is point number 3.

|11|   This is point number 4.
"
plot
finish
;----------------------- End -----------------------
