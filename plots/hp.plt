; hp.plt - example of x axis with unequally spaced tic marks 
;           and with word and phrases at tic marks
;
;-------------------------------------------------------------
%define XT	; <xlocation> <annotation>
; draw single tic mark on x axis
pltype=line : lintyp=0 : pltlwt=1.5*axlwt
xdata=$$1 : ydata=$$2 : zdata=0
_tl=0.012*(1+xlen/ylen)*(ymax-ymin) ; compute tic length
($1) (ymin)
($1) (ymin-_tl)
plot
; annotate tic mark
msgdat=y : mhalign=center : mvalign=top
($1) (ymin-_tl*1.5) "$2"
%%
;-------------------------------------------------------------
;
; plot frame without tic marks or annotations on x axis
yllc=3 : ylen=3 : yhor=y
xtic=0 : xanskp=-1
ttic=0 ; remove tic marks on top axis also
tlab=example of unequally-spaced tick marks
plot
;
; add tic marks and annotations on x axis
%XT 1 "word1"
%XT 3 "word2"
%XT 5.5 "This is a\nmulti-line\nphrase"
%XT 8.7 "This is\nanother\nphrase"

pltyp=symbol : symsiz=8 : symbol=22
5 5
plot
