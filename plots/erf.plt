; erf.plt
pltype=l : xper=90 : ticdir=in : head=n : clip=y
xlen=6 : ylen=4 : sizfac=6 : xint=5.2 : yint=5.2
ymax=1 : yhor=1 : yfmt=f.1 : yper=90
xlab=stimulus level (dB re threshold)
ylab=proportion of YES responses

%define     logit   1/(1+exp(-($1)))
%define     phi     (1+erf($1/sqrt(2)))/2
k=0.2
newframe
xmin=-40 : xmax=40 : xint=4.2 
xllc=2 : yllc=2  : xanskp=0 : yanskp=0 : 
ndata=81
xdata=(xmin)+($0-1)*(xmax-xmin)/(ndata-1)
ydata=logit($x/5)
zdata=0
lintyp=0
0
plot
lintyp=6
ydata=phi($x/8.65)
0
plot
0.5 3.2 "|_0| logit(x/5)"
0.5 2.6 "|_6| ^F^(x/8.65)"
