# Makefile for MinGW version of tplt

BIN = c:/usr/bin
CC=gcc
VERSION = local
CFLAGS = -Wall -O -fpack-struct -Dmgw -D_MSC_VER
WINLIB = -lwinmm -lkernel32 -luser32 -lgdi32 -lcomdlg32
OUTLIB = ../out/libout.a $(WINLIB) -lm

PLT_1= axanno.o axes1.o axes2.o axes3.o axes4.o axes5.o axgrid.o axline.o 
PLT_2= axtick.o bindat.o cnglun.o dattim.o defpar.o doplot.o errbar.o 
PLT_3= filetime.o getdat.o getmsg.o getnum.o getstr.o histo.o ifmt.o 
PLT_4= indata.o indx.o iscan.o labchr.o label.o labpos.o linetp.o 
PLT_5= linplt.o lookup.o namfil.o numb2.o nxtlin.o opninc.o pltkey.o 
PLT_6= polygon.o proces.o putmsg.o rect.o setaxpar.o setprm.o setsiz.o 
PLT_7= setup.o shadeare.o smooth.o sort.o stnmpm.o sttxpm.o wave.o
PLT_8= wrprml.o xpos.o wplt.o
PLT_O= $(PLT_1) $(PLT_2) $(PLT_3) $(PLT_4) $(PLT_5) $(PLT_6) $(PLT_7) \
       $(PLT_8)

PLT_C= axanno.c axes1.c axes2.c axes3.c axes4.c axes5.c \
	axgrid.c axline.c axtick.c \
	bindat.c cnglun.c dattim.c defpar.c doplot.c errbar.c \
	filetime.c getdat.c getmsg.c getnum.c getstr.c histo.c \
	ifmt.c indata.c indx.c iscan.c \
	labchr.c label.c labpos.c linetp.c linplt.c lookup.c \
	namfil.c numb2.c nxtlin.c opninc.c pltkey.c polygon.c \
	proces.c putmsg.c rect.c setaxpar.c setprm.c setsiz.c setup.c \
	shadeare.c smooth.c sort.c stnmpm.c sttxpm.c wave.c \
	wrprml.c xpos.c wplt.c

CFILES= tplt.c $(PLT_C)

HFILES= plt.h pltblk.h fort.h

all: tplt

clean:
	rm -f tplt.o *.o *~ tplt libplt.a *.exe *.EXE *.aps

install: tplt
	cp tplt.exe $(BIN)/plt.exe

tplt: tplt.o libplt.a ../out/libout.a
	gcc -o tplt wplt.o tplt.o libplt.a $(OUTLIB)

libplt.a: $(PLT_O)
	ar rus libplt.a $(PLT_1)
	ar rus libplt.a $(PLT_2)
	ar rus libplt.a $(PLT_3)
	ar rus libplt.a $(PLT_4)
	ar rus libplt.a $(PLT_5)
	ar rus libplt.a $(PLT_6)
	ar rus libplt.a $(PLT_7)
	ar rus libplt.a $(PLT_8)

../out/libout.a :
	make -C ../out libout.a

logo: tplt
	./tplt ../plots/logo

# This searches the C sources and makes dependence rules
depend:
	grep -i '^#include *"' $(CFILES) | \
	sed -e 's/:#include *"/: /' \
	    -e 's/"//' \
	    -e 's/\.c/.o/' \
	    -e 's,../[a-zA-Z]*/,,' | \
	awk ' { if ($$1 != prev) { print rec; rec = $$0; prev = $$1; } \
		else { if (length(rec $$2) > 78) { print rec; rec = $$0; } \
		       else rec = rec " " $$2 } } \
	      END { print rec } ' > makedep
	echo '$$r makedep' >>eddep
	echo '/^# DO NOT DELETE THIS LINE/+1,$$d' >eddep
	echo '$$r makedep' >>eddep
	echo 'w' >>eddep
	cp Makefile Makefile.bak
	ed - Makefile < eddep
	rm eddep makedep

# DO NOT DELETE THIS LINE

tplt.o: plt.h pltblk.h fort.h cincl.h version.h
wplt.o: wplt.h 
axanno.o: fort.h plt.h pltblk.h
axes1.o: plt.h pltblk.h fort.h
axes2.o: plt.h pltblk.h fort.h
axes3.o: plt.h pltblk.h
axes4.o: plt.h pltblk.h
axes5.o: plt.h pltblk.h
axgrid.o: plt.h pltblk.h fort.h
axline.o: plt.h pltblk.h fort.h
axtick.o: plt.h pltblk.h fort.h
bindat.o: plt.h pltblk.h cincl.h
cnglun.o: plt.h pltblk.h cincl.h
dattim.o: plt.h pltblk.h
defpar.o: plt.h pltblk.h cincl.h fort.h
doplot.o: plt.h pltblk.h fort.h
filetime.o: plt.h pltblk.h cincl.h fort.h
getmsg.o: plt.h pltblk.h fort.h
getstr.o: plt.h cincl.h
histo.o: plt.h pltblk.h
indata.o: plt.h pltblk.h fort.h
iscan.o: plt.h pltblk.h
labchr.o: plt.h pltblk.h
label.o: plt.h pltblk.h fort.h
labpos.o: plt.h pltblk.h fort.h
linplt.o: plt.h pltblk.h fort.h
lookup.o: plt.h pltblk.h
namfil.o: plt.h pltblk.h
numb2.o: plt.h pltblk.h fort.h
nxtlin.o: plt.h pltblk.h
opninc.o: plt.h pltblk.h cincl.h
pltkey.o: plt.h pltblk.h fort.h
polygon.o: plt.h pltblk.h
proces.o: plt.h pltblk.h fort.h
putmsg.o: plt.h pltblk.h fort.h
rect.o: plt.h pltblk.h
setaxpar.o: plt.h pltblk.h
setprm.o: plt.h pltblk.h fort.h
setsiz.o: plt.h pltblk.h
setup.o: plt.h pltblk.h cincl.h fort.h
shadeare.o: plt.h pltblk.h
stnmpm.o: plt.h pltblk.h fort.h
sttxpm.o: plt.h pltblk.h fort.h
wave.o: plt.h pltblk.h fort.h
wrprml.o: plt.h pltblk.h
xpos.o: plt.h pltblk.h
# (makedep will replace everything after this line) 

axanno.obj:#include "fort.h" "plt.h" "pltblk.h"
axes1.obj:#include "plt.h" "pltblk.h" "fort.h"
axes2.obj:#include "plt.h" "pltblk.h" "fort.h"
axes3.obj:#include "plt.h" "pltblk.h"
axes4.obj:#include "plt.h" "pltblk.h"
axes5.obj:#include "plt.h" "pltblk.h"
axgrid.obj:#include "plt.h" "pltblk.h" "fort.h"
axline.obj:#include "plt.h" "pltblk.h" "fort.h"
axtick.obj:#include "plt.h" "pltblk.h" "fort.h"
bindat.obj:#include "plt.h" "pltblk.h" "cincl.h"
cnglun.obj:#include "plt.h" "pltblk.h" "cincl.h"
dattim.obj:#include "plt.h" "pltblk.h"
defpar.obj:#include "plt.h" "pltblk.h" "cincl.h" "fort.h"
doplot.obj:#include "plt.h" "pltblk.h" "fort.h"
filetime.obj:#include "plt.h" "pltblk.h" "cincl.h" "fort.h"
getmsg.obj:#include "plt.h" "pltblk.h" "fort.h"
getnum.obj:#include "fort.h"
getstr.obj:#include "plt.h" "pltblk.h" "cincl.h"
histo.obj:#include "plt.h" "pltblk.h"
indata.obj:#include "plt.h" "pltblk.h" "fort.h"
iscan.obj:#include "plt.h" "pltblk.h"
labchr.obj:#include "plt.h" "pltblk.h"
label.obj:#include "plt.h" "pltblk.h" "fort.h"
labpos.obj:#include "plt.h" "pltblk.h" "fort.h"
linplt.obj:#include "plt.h" "pltblk.h" "fort.h"
lookup.obj:#include "plt.h" "pltblk.h"
namfil.obj:#include "plt.h" "pltblk.h"
numb2.obj:#include "plt.h" "pltblk.h" "fort.h"
nxtlin.obj:#include "plt.h" "pltblk.h"
opninc.obj:#include "plt.h" "pltblk.h" "cincl.h"
pltkey.obj:#include "plt.h" "pltblk.h" "fort.h"
polygon.obj:#include "plt.h" "pltblk.h"
proces.obj:#include "plt.h" "pltblk.h" "fort.h"
putmsg.obj:#include "plt.h" "pltblk.h" "fort.h"
rect.obj:#include "plt.h" "pltblk.h"
setaxpar.obj:#include "plt.h" "pltblk.h"
setprm.obj:#include "plt.h" "pltblk.h" "fort.h"
setsiz.obj:#include "plt.h" "pltblk.h"
setup.obj:#include "plt.h" "pltblk.h" "cincl.h" "fort.h"
shadeare.obj:#include "plt.h" "pltblk.h"
stnmpm.obj:#include "plt.h" "pltblk.h" "fort.h"
sttxpm.obj:#include "plt.h" "pltblk.h" "fort.h"
tplt.obj:#include "plt.h" "pltblk.h" "fort.h" "cincl.h" "version.h"
wave.obj:#include "plt.h" "pltblk.h" "fort.h"
wplt.obj:#include "wplt.h" "output.h"
wrprml.obj:#include "plt.h" "pltblk.h"
xpos.obj:#include "plt.h" "pltblk.h"
plt.h:#include "output.h"
