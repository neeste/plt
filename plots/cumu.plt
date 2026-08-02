; cumu.plt - example of a cumulative distribution plot
;
header=0 : tlab=cumulative distribution
pltyp=cum : smooth=n : cdext=y
xper=90 : yper=90 : grid=y : yhor=y : ticdir=in
ndata=20
xdata=$1*($random+$random+$random+$random+$random)
ymax=10
norm=(ymax)
2
keep
smooth=y : pltcol=1
plot
