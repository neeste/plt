; fig3 - Test patterns to demonstrate area fill options.
;
head=no
;
axlwt=0 : pltlwt=0.6 : annlwt=0
xlen=10 : ylen=10 : xllc=0 : yllc=0
wxlen=1 : wylen=1 : magnify=0.8
pltype=6
mhal=center : mval=half : msgdat=1
msgang=0 : msgsiz=-0.2

data
0    1
1    1
1    0
0.55 0
0.55 0.3
0.7  0.3
0.7  0.7
0.3  0.7
0.3  0.3
0.45 0.3
0.45 0
0    0

%repeat 40
shade=$0-1 : vxllc=1+floor(shade/5) : vyllc=1.75+shade%5
%msg 0.5 0.5 "%.0f" shade
keep
%%
