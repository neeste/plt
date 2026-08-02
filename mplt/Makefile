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
HELP_DIR=Plt.help
HELP_IDX=Plt.help/Contents/Resources/PltHelp.helpindex 

TPLTF = axanno.c axes1.c axes2.c axes3.c axes4.c axes5.c axgrid.c axline.c \
	axtick.c bindat.c cincl.h cnglun.c dattim.c defpar.c doplot.c \
	errbar.c filetime.c fort.h getdat.c getmsg.c getnum.c getstr.c \
	histo.c ifmt.c indata.c indx.c iscan.c labchr.c label.c labpos.c \
	linetp.c linplt.c lookup.c namfil.c numb2.c nxtlin.c opninc.c \
	output.h plt.h pltblk.h pltkey.c polygon.c proces.c putmsg.c rect.c \
	setaxpar.c setprm.c setsiz.c setup.c shadeare.c smooth.c sort.c \
	stnmpm.c sttxpm.c tplt.c version.h wave.c wrprml.c xpos.c
OUT_F = area.c clip.h copa.h cview.h cwind.h d0.c d1.c d2.c d3.c d5.c \
	d6.c d7.c d8.c d9.c dbind.h device.h dranodes.c dshlin.c emf.h \
	font.h font0.c font11.c fort.h gdi.h getarc.c getbox.c hatchare.c \
	hollowar.c inqopa.c line.c marker.c md.c opnout.c otrans.c \
	output.c output.h penwid.c pythag.c resetout.c \
	setopa.c sindeg.c skqtxx.c sktext.c solidare.c sound.c symbol.c \
	text.c tpcl.c tppcl.c winout.h

version:
	perl version.pl

build: version 
	rm -rf build
	mkdir -p build/Release/Plt.app/Contents/MacOS
	mkdir -p build/Release/Plt.app/Contents/Resources
	clang -O2 -framework Cocoa -Wstrict-prototypes -Wdeprecated-non-prototype -o build/Release/Plt.app/Contents/MacOS/Plt macplt.m d4mac.c $$(echo "$(TPLTF)" | tr ' ' '\n' | grep '\.c$$' | sed 's|^|tplt/|') $$(echo "$(OUT_F)" | tr ' ' '\n' | grep '\.c$$' | sed 's|^|out/|') -I. -I./out
	@echo '<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd"><plist version="1.0"><dict><key>CFBundleExecutable</key><string>Plt</string><key>CFBundleIdentifier</key><string>com.steveneely.Plt</string><key>CFBundleName</key><string>Plt</string><key>CFBundlePackageType</key><string>APPL</string><key>CFBundleShortVersionString</key><string>1.0</string><key>CFBundleVersion</key><string>1</string><key>NSPrincipalClass</key><string>NSApplication</string><key>CFBundleHelpBookFolder</key><string>Plt.help</string><key>CFBundleHelpBookName</key><string>Plt Manual</string></dict></plist>' > build/Release/Plt.app/Contents/Info.plist
	cp -R Plt.help build/Release/Plt.app/Contents/Resources/
	xattr -cr build/Release/Plt.app
	codesign -s - --force --deep build/Release/Plt.app

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

