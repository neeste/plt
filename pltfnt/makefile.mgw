# Makefile for pltfnt on DOS using MinGW

BIN = c:/usr/bin
FNTDIR=c:/fonts
CFLAGS = -Wall -O -fpack-struct -Dmgw -DMSDOS
WINLIB = -lwinmm -lkernel32 -luser32 -lgdi32 
LIBS = ../out/libout.a $(WINLIB) -lm
CC=gcc

%.pft : %.txt
	pltfnt -p $*.txt

%.c   : %.txt
	pltfnt -c $*.txt

FONTS= altfnt.pft audsym.pft cmpgrk.pft cmpita.pft cmprom.pft \
cmpscr.pft duprom.pft gtheng.pft gthger.pft \
oldalt.pft oldstd.pft oldsym.pft smpgrk.pft smprom.pft \
smpscr.pft stdfnt.pft symfnt.pft trpita.pft trprom.pft \
cmpitaii.pft cmpromii.pft dupromii.pft smpromii.pft stdfntii.pft \
trpitaii.pft trpromii.pft altbis.pft stdbis.pft ipa.pft


pltfnt: pltfnt.o font.h
	gcc $(CFLAGS) -o pltfnt pltfnt.o $(LIBS)

txx: txx.o
	gcc $(CFLAGS) -o txx txx.o $(LIBS)

all : $(FONTS)

install_exe : pltfnt 
	cp pltfnt.exe $(BIN)

install_fnt : $(FONTS) $(FNTDIR)
	cp *.pft $(FNTDIR)

install : install_exe install_fnt

$(FNTDIR) :
	mkdir -p $(FNTDIR)

txt : stdfnt.txt altfnt.txt symfnt.txt

stdfnt.txt : pltfnt
	pltfnt -f0 -t > stdfnt.txt

altfnt.txt : pltfnt
	pltfnt -f1 -t > altfnt.txt

symfnt.txt : pltfnt
	pltfnt -f2 -t > symfnt.txt

pft : stdfnt.pft altfnt.pft symfnt.pft

stdfnt.pft : pltfnt stdfnt.txt
	pltfnt -f0 -p stdfnt

altfnt.pft : pltfnt altfnt.txt
	pltfnt -f1 -p altfnt

symfnt.pft : pltfnt symfnt.txt
	pltfnt -f2 -p symfnt

clean:
	rm -f core *.o *.exe *.pft *~ pltfnt txx
