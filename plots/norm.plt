; norm.plt - plot Gaussian curves
;
head=0 : annlwt=0 : ndata=100 : e=2.718281828
xdata=$0*(xmax/ndata)
ydata=$3+$4*e^(-(($x-$1)/$2)^2)
zdata=0
pltyp=lines
4 2 3 5     ; <mean> <s.d.> <baseline> <height>
plot
6 2 3 5     ; <mean> <s.d.> <baseline> <height>
plot
