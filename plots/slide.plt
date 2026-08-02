;-------------------------->>> Begin <<<-------------------------------
;
;                     PLT File Name: slide.plt
;
;      (Example of a plt file to make a slide with centered text.)
;
;         Name:    W.J. Kelly           Date: 15-Feb-89
;
;-------------->>>  Description of Frame and Axes  <<<-----------------
;
stdfnt trprom
lablwt  = 0.0
annlwt  = 0.0
axlwt   = 0.0
header  = NO
mhalign = CENTER
;--------------------->>>  Title of Slide  <<<--------------------------
;
msgsiz  =  2.0   : mxllc   =  3.0  : myllc   = 5.2
"TITLE OF SLIDE"
;---------------------------------------------------
;              >>>  Text. <<<
;
msgsiz  =  2.0   : mxllc   =  3.0  : myllc   = 3.0
"This is an example
of a text slide with
each line centered."
;----------------------- End -----------------------
