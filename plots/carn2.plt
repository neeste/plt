;========================================================================
;A question has arisen.  Janet Fitzakerly wanted to create color plots
;with different color symbols.  She wanted the color of the symbol
;embedded in a message to correspond to each color.  If each
;message is plotted right after the data are plotted, the color will
;match.  Otherwise the last pltcol governs the colors of the embedded
;symbols. 
;
;I tried plotting each message separately and placing a new
;value for pltcol in between, but the pltcol has no effect.  I
;thought that a plot keyword might activate the color, but
;that didn't work either.
;
;The solution that works is to plot each message after each set of data
;are plotted (as I said above).  The difficulty is, of course, aligning
;the messages up with each other--not impossible, admittedly, but still
;tedious. 
;
;Any suggestions?
;
;Here's the file
;========================================================================
;
; OVERALL SETUP
;
header=no
bgcol=1
fgcol=15
shdcol=7
;
; FIRST, THE X AXIS
;
xlength=7.0 : xpercent=60 : ypercent=60
xllc=2.0 : xmin=1.0 : xmax=5.0 : xcycle=0 : xint = 4
xlabel=						; NO LABEL
;
; USE THE STRING REPLACEMENT FEATURE OF xannot
annsize=.90
xannot= 1,DMV 2,USC 3,CPU 4,HUD 5,LED
;
; NOW, THE Y AXIS
;
ylength=5.0 : yllc=1.75 : ymin=0.0 : ymax=100.0 : yint=4 : yhor=yes
ylabel = Y-axis Label
ticdir=inward : ticsiz=0.5 : clip=yes
;
; MAIN LABEL
;
msgsiz=1.3
0.1,5.3,"Main Label (0.1,5.3)|59| MSGSIZE = 1.3"
;
; LABEL FOR THE X-AXIS
;
msgsiz=1.1 : mhalign=center
3.5,-0.75,"X-Axis Label (2.5,-0.75)|59| MSGSIZ = 1.1"
;
; NOW, THE DATA VALUES
;
symbol=10 : symsiz= 0.75
pltype=both : lintype= 0
pltcol=12
data
1        65.0
2        74.0
3        65.0
4        76.0
5        57.0
plot
msgsiz=0.75 : msgcol=7 : mhalign=left
1.0,4.25,"|10| Mode 1"
symbol=11
pltcol=13
data
1        15.0
2        26.0
3        35.0
4        24.0
5        32.0
plot
1.0,4.05,"|11| Mode 2"
symbol=12
pltcol=14
data
1        20.0
2        00.0
3        00.0
4        00.0
5        11.0
plot
1.0,3.85,"|12| Mode 3"
;
; LEGEND BOX
;
pltype=rect : pltlwt=.75
pltcol=15
data
0.50 115
1.8  92
plot
finish



