pltype=C : xper=90 : ticdir=in
ymax=1 : yhor=1 : yfmt=f.1 : yper=90 : norm=1
ndata=100
lintype=8
%define rf $random+$random+$random+$random-2
xdata=rf(0)+rf(0)+rf(0)+rf(0)+4
0 0
plot
lintype=0
xdata=rf(0)+rf(0)+rf(0)+rf(0)+6
0 0
plot
