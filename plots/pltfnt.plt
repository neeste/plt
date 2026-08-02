; pltfnt.plt - demonstrate use of external character fonts
;
header=0 : sizfac=8
stdfnt cmprom	; Complex Roman Font
xllc=7 : xlen = 2 : xanskp=1 : xmin=0 : xmax=8 : xint=8
yllc=5 : ylen = 2 : yanskp=1 : ymin=0 : ymax=8 : yint=8
xlabel=x-axis
ylabel=y-axis
rlabel=right axis
tlabel=top axis

pltyp=symbols : symbol=22 : symsiz=4
4,4
plot

stdfnt smprom	; Simplex Roman Font
altfnt cmpscr	; Complex Script Font
-6.5,2.5,"
The figure on the right uses the
\"complex roman\" font for the
axis labels.  This is done by
putting the line

    stdfnt cmprom

in the pltfile preceding the plot-
ting of the axes.  This message
is plotted in the \"simplex roman\"
font.

The alternate font (which is normally used for greek
characters) can also be changed to an external font.
For example, \"altfnt cmpscr\" changes the alternate
font to ^complex script^.  For a complete list of external
character fonts currently available type

    plt -i /usr/src/local/plt/plots/fonts

to plot font samples on the Imagen printer.
"
