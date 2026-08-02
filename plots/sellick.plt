;Cambridge 5.11.1994
rotate=no : header = no
ticdir=-1
axlwt=1 : pltlwt=1.5 : msglwt=1 : lablwt=1 : annlwt=1
yllc=1 : xllc=3 : ylen=6 : xlen=4 : labsize=1.2
ypercent=90 : ymin=0 : ymax=90 : yint=9 : ypercent=90
yannot=0 10 20 30 40 50 60 70 80 90 : ylabel= Sound level needed for criterion velocity, dB SPL
yhor=y
xmin=0.8 : xmax=30 : xpercent=100 : xcycle=10 : xannot=1 2 5 10 20
xlabel= Frequency, kHz
plot
symsize=1.4
pltype=3 : lintype=0 : symbol= 1
data; krzywa strojenia
1 75
3 72
7 68
10 59
13 44
16 28
18 12
19 27
20 52
20.5 83
plot

pltype=3 : lintype=0 : symbol= 10
data; krzywa strojenia po smierci
1 71
3 71.5
7 65
10 52
13 51
17 78
19 91
plot

msgsiz= 1.1
msgdat=y : mhkey=2
1,5,"|_0||1||_0| Start of experiment"
1,12,"|_0||10||_0| Post mortem"

