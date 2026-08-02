# PLT makefile for MAC

BINDIR=/usr/local/bin
ARC=../archive
DST=../dist

build:
	sudo make -C mplt clean
	make -C mplt build
	echo Compilation complete.

install: build
	sudo make -C mplt install
	echo Installation complete.

clean:
	sudo make -C mplt clean

empty: clean

version:
	 make -C mplt clean

dist: 
	sudo make -C mplt dist

