# PLT makefile for Mac
#
# make package requires installation of Packages
# http://s.sudre.free.fr/Software/Packages/about.html

APP=~/Applications
ARC=../../archive
BLD=build/Release
DST=../../dist
PKG=package/Plt.pkg
PM=/usr/local/bin/packagesbuild
HELP_DIR=MacPlot/Plt.help
HELP_IDX=MacPlot/Plt.help/Contents/Resources/PltHelp.helpindex 

TPLTF = axanno.c axes1.c axes2.c axes3.c axes4.c axes5.c axgrid.c axline.c \
	axtick.c bindat.c cincl.h cnglun.c dattim.c defpar.c doplot.c \
	errbar.c filetime.c fort.h getdat.c getmsg.c getnum.c getstr.c \
	histo.c ifmt.c indata.c indx.c iscan.c labchr.c label.c labpos.c \
	linetp.c linplt.c lookup.c namfil.c numb2.c nxtlin.c opninc.c \
	output.h plt.h pltblk.h pltkey.c polygon.c proces.c putmsg.c rect.c \
	setaxpar.c setprm.c setsiz.c setup.c shadeare.c smooth.c sort.c \
	stnmpm.c sttxpm.c tplt.c version.h wave.c wrprml.c xpos.c
OUT_F = area.c clip.h copa.h cview.h cwind.h d0.c d1.c d2.c d3.c d4.c d5.c \
	d6.c d7.c d8.c d9.c dbind.h device.h dranodes.c dshlin.c emf.h \
	font.h font0.c font11.c fort.h gdi.h getarc.c getbox.c hatchare.c \
	hollowar.c inqopa.c line.c marker.c md.c opnout.c otrans.c \
	outlib.c output.c output.h penwid.c pythag.c resetout.c \
	setopa.c sindeg.c skqtxx.c sktext.c solidare.c sound.c symbol.c \
	text.c tpcl.c tppcl.c winout.h

version:
	perl version.pl

build: version 
	rm -rf build
	xcodebuild CODE_SIGN_IDENTITY="" CODE_SIGNING_REQUIRED=NO CODE_SIGNING_ALLOWED=NO MACOSX_DEPLOYMENT_TARGET=10.13
	xcodebuild -target Plt CODE_SIGN_IDENTITY="" CODE_SIGNING_REQUIRED=NO CODE_SIGNING_ALLOWED=NO MACOSX_DEPLOYMENT_TARGET=10.13
	xcodebuild -target TestOut CODE_SIGN_IDENTITY="" CODE_SIGNING_REQUIRED=NO CODE_SIGNING_ALLOWED=NO MACOSX_DEPLOYMENT_TARGET=10.13

package: build 
	rm -rf package/BTNRH
	mkdir package/BTNRH
	cp logo.plt package/BTNRH
	cp -R build/Release/Plt.app package/BTNRH
	$(PM) package/Plt.pkgproj

install: package
	installer -pkg $(PKG) -target /

zipmac: package
	zip -jr plt-mac $(PKG)

dist: zipmac 
	mkdir -p $(DST) $(ARC)
	mv -f plt-*.zip $(DST)
	cp $(DST)/plt*.zip $(ARC)

update:
	cd ../tplt; cp -p $(TPLTF) ../mplt/tplt
	cd ../out ; cp -p $(OUT_F) ../mplt/out

help:
	hiutil -Caf $(HELP_IDX) $(HELP_DIR)

clean:
	rm -f *~ package/*.pkg
	rm -rf package/BTNRH
	rm -rf build % requires sudo in MacOS

