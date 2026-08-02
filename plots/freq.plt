; freq.plt - example of a cumulative distribution plot
;
header=0 : tlab=frequency distribution
pltyp=freq
xper=90 : yper=90 : grid=y : yhor=y : ticdir=in : xint=10.5
ndata=100
xdata=$1*($random+$random+$random+$random+$random)
ymax=10
norm=(ymax*10)
2
plot
