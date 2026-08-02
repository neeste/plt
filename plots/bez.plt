; bez.plt - Bezier fit to circular arc
;
a=120 : r=5 : ppi=45 
;a=-90 ; test negative angles
axlwt=0 : annlwt=0 : lablwt=0 : pltlwt=3 : xllc=3.5 : symsiz=0.8
dr=atan(1)/45 : xmax=(xlen) : ymax=(ylen)
;
%define ph	(($1-1)/(ndat-1))*(a*dr)
%define sign	ifelse($1<0,-1,1)
;
;---------------------------------------------
;
; end-points
;
p1x=(r)           : p1y=0
p2x=(r*cos(dr*a)) : p2y=(r*sin(dr*a))
;
; mid-point
;
b=1/tan(dr*a/2)
m=[sign(a)*sqrt(1+b^2)-b]
;
pmx=[p1x+p2x+(p2y-p1y)*m]/2 ; Pm=[P1+P2+(P2-P1)*m*R0]/2
pmy=[p1y+p2y-(p2x-p1x)*m]/2
;
; control points
;
c=cos(dr*a)
s=sin(dr*a)
d=m*2/3
e=d*s/(1-c)
;
pax=p1x+(p2y-p1y)*d+(p2x-p1x)*e ; Pa=P1+(P2-P1)*(4/3)*m*R0/(1-R1)
pay=p1y+(p2y-p1y)*e-(p2x-p1x)*d
pbx=p2x+(p1x-pax)*c-(p1y-pay)*s ; Pb=P2+(P1-Pa)*R1
pby=p2y+(p1x-pax)*s+(p1y-pay)*c
;
;---------------------------------------------
;
; set plotting limits
;
mx=(min(p1x,p2x,pax,pbx)+max(p1x,p2x,pax,pbx))/2
my=(min(p1y,p2y,pay,pby)+max(p1y,p2y,pay,pby))/2
xllc=5-mx : yllc=4-my
;
; plot end-points
;
pltyp=sym
(p1x) (p1y)
(p2x) (p2y)
plot
;
; plot arc
;
pltype=line : ndat=100
xdat=r*cos(ph($0)) : ydat=r*sin(ph($0))
0 0
plot
ndat=1 : xdat=$1 : ydat=$2
;
; plot midpoint
;
pltcol=2 : pltyp=s : pltlwt=0.8 : symb=11
(pmx) (pmy)
plot
;
; plot control points
;
pltcol=4 : pltyp=b : pltlwt=0.8 : symb=1
(p1x) (p1y)
(pax) (pay)
(pbx) (pby)
(p2x) (p2y)
plot
;
;plot Bezier
;
ndat=floor(max(2,ppi*r*abs(a)/360)+0.5)
%define tt ($1-1)/(ndat-1)
%define t1 (1-tt($1))^3
%define ta 3*tt($1)*(1-tt($1))^2
%define tb 3*tt($1)^2*(1-tt($1))
%define t2 tt($1)^3
xdat=(t1($0)*p1x+ta($0)*pax+tb($0)*pbx+t2($0)*p2x)
ydat=(t1($0)*p1y+ta($0)*pay+tb($0)*pby+t2($0)*p2y)
pltcol=12 : pltype=both : symsiz=0.5
0 0
plot
mhal=cen : mval=h
%msg mx/2 my/2 "a=%.0f  n=%.0f" a ndat
