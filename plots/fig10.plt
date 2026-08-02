; fig10.plt - Example 4
;
; Figure 8 from "Ten Graphs (and How to Use Them)"
; 		PC World, September, 1988, page 216
;
head=0
xllc=1.75  : xlen=5 : xmin=0 : xmax=4   : xint=0
yllc=1.125 : ylen=5 : ymin=0 : ymax=1.5 : yint=15
yhor=yes : yanskp=1 : yfmt=f.1 : ticdir=in : annsiz=1.2

%define box     ; <xloc> <min> <10%> <25%> <50%> <75%> <90%> <max>
pltyp=symb : symbol=1
($1) $2	                ; min
($1) $8	                ; max
plot
pltyp=rect : ebnum=1 : ebwid=2
w=0.25                  ; half-width of box
($1-w) $4 ($4-$3)       ; 10 & 25 percentile
($1+w) $5               ; 50 percentile
($1-w) $6 ($7-$6)       ; 75 & 90 percentile
($1+w) $5               ; 50 percentile
plot
%%

u=1 : t=2 : j=3 : k=6.25
xanno= u,US t,TAIWAN j,JAPAN

;       min  10%  25%  50%  75%  90%  max
%box u 0.15 0.19 0.30 0.48 0.57 0.62 0.75 ; US
%box t 0.65 0.75 0.85 0.95 1.03 1.08 1.15 ; TAIWAN
%box j 0.80 0.96 1.02 1.12 1.20 1.30 1.40 ; JAPAN
%box k 0.80 0.90 1.00 1.10 1.20 1.30 1.40 ; KEY

m=k-0.4 : p=-1/72
msgdat=yes : mhalign=right : mvalign=half : msgsiz=12*p
(m) 1.4 "maximum"
(m) 1.3 "90th percentile"
(m) 1.2 "75th percentile"
(m) 1.1 "50th percentile"
(m) 1.0 "25th percentile"
(m) 0.9 "10th percentile"
(m) 0.8 "minimum"

stdfnt trprom
altfnt trpita
msgdat=no : mhalign=center : mvalign=bottom : msgsiz=12*p : m=0.1
0 ylen+m "Standard Score"
mxdat=no :  mhalign=center : mvalign=base : msgsiz=20*p
3.5 6.250 "Japanese 5th Graders Excel in Math"
msgsiz=12*p
3.5 5.875  "Achievement Test Scores for 20 Classrooms"
mhalign=right : mvalign=bottom : msgsiz=8*p : m=0.1-yllc
8.5 m "Redrawn from H. Stevenson, ^et al.^ (1986). Science 231, 693-699."

