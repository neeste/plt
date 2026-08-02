# makefile to build the library "libout.a" using MinGW compiler

CFLAGS = -Wall -O -fpack-struct -Dmgw -D_MSC_VER
LNKLST = tstout.o libout.a
WINLIB = -lwinmm -lkernel32 -luser32 -lgdi32 
LIBS = $(WINLIB) -lm
LIB = c:\usr\lib
CC=gcc

OBJ1 = d0.o d1.o d2.o d3.o d4win32.o d5.o d6.o d7.o d8.o d9.o
OBJ2 = area.o dranodes.o dshlin.o font0.o
OBJ3 = font11.o getarc.o getbox.o hatchare.o hollowar.o inqopa.o
OBJ4 = line.o marker.o opnout.o otrans.o output.o penwid.o pythag.o
OBJ5 = resetout.o setopa.o sktext.o skqtxx.o solidare.o symbol.o text.o
OBJ6 = tpcl.o tppcl.o sindeg.o md.o sound.o
OUT_O= $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6)
OUT_C= d0.c d1.c d2.c d3.c d4.c d5.c d6.c d7.c d8.c d9.c \
	area.c dranodes.c dshlin.c font0.c \
	font11.c getarc.c getbox.c hatchare.c hollowar.c inqopa.c \
	line.c marker.c opnout.c otrans.c output.c penwid.c pythag.c \
	resetout.c setopa.c sktext.c skqtxx.c solidare.c symbol.c text.c \
	tpcl.c tppcl.c sindeg.c md.c sound.c

build: tstout 

install: libout.a
	cp libout.a $(LIB)

libout.a: $(OUT_O)
	ar ru libout.a $(OBJ1)
	ar ru libout.a $(OBJ2)
	ar ru libout.a $(OBJ3)
	ar ru libout.a $(OBJ4)
	ar ru libout.a $(OBJ5)
	ar ru libout.a $(OBJ6)
	ranlib libout.a

tstout: $(LNKLST)
	gcc -mwindows winout.c -o tstout $(LNKLST) $(LIBS)

test: tstout
	tstout

clean:
	rm -f *.a *.o *.obj *.bak *.log *.exe *.res *.lib *.dll *~ tstout *.aps

# dependencies

area.o: copa.h
d0.o: fort.h copa.h device.h output.h
d1.o: device.h copa.h output.h
d2.o: device.h copa.h output.h clip.h
d3.o: device.h copa.h output.h
d4x.o: device.h copa.h output.h
d5.o: device.h copa.h clip.h output.h
dranodes.o: font.h
font0.o: font.h
font11.o: font.h
hatchare.o: copa.h cview.h
hollowar.o: copa.h device.h
inqopa.o: output.h fort.h clip.h copa.h device.h cview.h cwind.h
line.o: copa.h cview.h
marker.o: copa.h cview.h device.h font.h
opnout.o: copa.h device.h font.h
otrans.o: output.h fort.h clip.h copa.h cview.h cwind.h
output.o: output.h fort.h font.h clip.h copa.h device.h cview.h cwind.h
penwid.o: copa.h device.h cview.h
resetout.o: output.h clip.h copa.h device.h cview.h cwind.h font.h
setopa.o: output.h fort.h clip.h copa.h device.h cview.h cwind.h
sktext.o: output.h copa.h cview.h device.h font.h
skqtxx.o: output.h copa.h cview.h device.h font.h
solidare.o: copa.h cview.h device.h
symbol.o: cview.h device.h font.h
text.o: output.h copa.h cview.h device.h
tpcl.o: output.h clip.h device.h copa.h cview.h cwind.h

