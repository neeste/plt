# Makefile for plt manual page

MANSYS=pltlnx.man
MANSUF=1
MANDIR=/usr/local/share/man/man$(MANSUF)
SED=sed -e '/UNIX/d' -e 's/.\//g'

all: pltlnx.man pltpc.txt

pltpc.man : pltpc_1.man plt_2.man pltpc_3.man
	cat pltpc_1.man plt_2.man pltpc_3.man > pltpc.man

pltlnx.man : pltlnx_1.man plt_2.man pltlnx_3.man
	cat pltlnx_1.man plt_2.man pltlnx_3.man > pltlnx.man

pltpc.txt : pltpc.man
	nroff -man pltpc.man | $(SED) | cat -s > pltpc.txt

man: $(MANSYS)
	nroff -man $(MANSYS) | more -s

$(MANDIR) :
	mkdir -p $(MANDIR)

print: $(MANSYS)
	groff -man -Tps $(MANSYS) | lpr

install: $(MANSYS) $(MANDIR)
	#install -m 755 $(MANSYS) $(MANDIR)/plt.$(MANSUF)
	cp -f $(MANSYS) $(MANDIR)/plt.$(MANSUF)

clean:
	rm -f core pltlnx.man pltlnx.txt pltpc.man
