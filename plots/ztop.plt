; ztop - test function relating z-score to percent score
pltyp=line : yhor=y : tlab=z to p
xmin=-4 : xmax=4 : xint=8 : xlab=z score
ymin=0 : ymax=100 : ylab=percent score
ndata=100
xdata=xmin+(xmax-xmin)*($0-1)/(ndata-1)
ydata=100*ztop($x) : lintyp=1
data
0
plot
%define     phi     (1+erf($1/sqrt(2)))/2
ydata=100*phi($x-1) : lintyp=2
data
0
plot
