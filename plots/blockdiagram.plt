; blockdiagram.plt
;
head=no
xaxis=0 : yaxis=0 : taxis=0 : raxis=0
pltyp=rect
1.0 1
2.5 7
plot
pltyp=rect
3.5 1
5.0 7
plot
pltyp=sym : symbol=1 : symsiz=5
8.5 4
plot
pltyp=line : aronum=1
-0.5 4
 1.0 4
plot
2.5 6.5
3.5 6.5
plot
2.5 5.5
3.5 5.5
plot
2.5 4.5
3.5 4.5
plot
2.5 1.5
3.5 1.5
plot
9.02  4
10.5 4
plot
pltyp=line : aronum=0
5.0 6.5
6.0 6.5
plot
5.0 5.5
6.0 5.5
plot
5.0 4.5
6.0 4.5
plot
5.0 1.5
6.0 1.5
plot
pltyp=line : aronum=1
6.0 6.51
8.1 4.39
plot
6.0 5.5
8.01 4.25
plot
6.0 4.5
7.95 4.1
plot
6.0 1.51
8.1 3.61
plot
pltyp=sym
symbol=11 : symsiz=0.5
3 2.5
3 3.0
3 3.5
5.5 2.5
5.5 3.0
5.5 3.5
plot
mhal=cen : mval=half : msgdat=yes : msgsiz=1.2
 0   4.5 "input"
10   4.5 "output"
msgsiz=2
8.5  4   "+"
msgang=90 : msgsiz=1.5
1.75 4.0 "filterbank"
4.25 4.0 "suppressor"
