; roex.plt - plot integral of roex (p,r) filter functions
;
; X-axis is G, relative gap width.
; Y-axis is masked threshold.
;
; P is represented by $1; R by $2, and K by $3.
; Following the convention used by Glasberg, P and R are assumed to
; be in dB.
;
;---------------------------------------------------------------
; For left half, Mb > Ma, so final term is just Mb ($1).
;
;      TEMP=[(R-1.0) * (2.+P) * EXP(-P)/P +R]
;      PSUB=2.*(TEMP-((R-1.0)*(2.+P*G)*EXP(-P*G)/P+R*G))
;
%define temp    "($2-1) * (2+$1*$3) * e^(-$1*$3)/$1 +$2*$3"
%define db      10*log($1)
%define roex    ; <R> <P> <G>
e=2.718281828
ndata=50
xdata=$$0*(xmax/ndata)
ydata=$$3 + db(2*[temp($$1,$$2,1) - temp($$1,$$2,$$x)])
zdata=0
$1 $2 $3
plot
ndata=1
xdata=
ydata=
zdata=
%%
;---------------------------------------------------------------
head=0
clip=yes
pltyp=lines
ymin=0
ymax=60
xmin=0
xmax=0.9999
xper=90
yper=90
ylabel = Masked Threshold (dB SPL)
xlabel= Relative Gap Width
yhor=yes
yint=3.2
xint=4.2
xfmt=f.1
ticdir=in
yper=90

%roex 25 .0001 60
