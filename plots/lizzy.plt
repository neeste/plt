; lizzy.plt - Lissajous figure
pltyp=line : yhor=y : ticdir=in : grid=y
a=17 : b=23 : c=6/7
xmin=20 : xmax=70 : xint=5.2 : xper=(5/6)*100 : xlen=7*c
ymin=40 : ymax=70 : yint=3.2 : yper=(3/4)*100 : ylen=5*c
ndata=1000 : t=2*3.1415927
xdata=(xmin+(xmax-xmin)*(sin(a*t*($0-1)/(ndata-1))+1)/2) : xlab=L[2]
ydata=(ymin+(ymax-ymin)*(cos(b*t*($0-1)/(ndata-1))+1)/2) : ylab=L[1]
data
0
plot
