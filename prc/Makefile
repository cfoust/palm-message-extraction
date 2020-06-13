#!make
#
# The contents of this file are subject to the Mozilla Public
# License Version 1.1 (the "License"); you may not use this file
# except in compliance with the License. You may obtain a copy of
# the License at http://www.mozilla.org/MPL/
# 
# Software distributed under the License is distributed on an "AS
# IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
# implied. See the License for the specific language governing
# rights and limitations under the License.
# 
# The Original Code is par & libprc, released May 13, 1999.
# 
# The Initial Developer of the Original Code is David Williams,
# http://www.djw.org.  Portions created by David Williams are
# Copyright (C) 1999 David Williams. All Rights Reserved.
# Contributor(s):
#
# par(1) see usage.
#
# created: djw@djw.org, January 2, 1999.
#
INSTALL_INCDIR = /usr/local/pilot/include
INSTALL_LIBDIR = /usr/local/pilot/lib
INSTALL_BINDIR = /usr/local/bin
INSTALL_MANDIR = /usr/local/man/man1
CC =     gcc
CFLAGS = -g -I. -Wall
LD =     gcc
LDFLAGS = -g
AR      = ar
RANLIB  = ranlib

CFILES = \
	create.c   \
	open.c     \
	close.c    \
	destroy.c  \
	ihead.c    \
	flush.c    \
	fpopen.c   \
	frhead.c   \
	fwhead.c   \
	frrec.c    \
	fwrec.c    \
	frrsrc.c   \
	fwrsrc.c   \
	recapp.c   \
	recget.c   \
	recset.c   \
	recrem.c   \
	recdes.c   \
	rsrcapp.c  \
	rsrcget.c  \
	rsrcnext.c \
	recnext.c  \
	rsrcset.c  \
	rsrcrem.c  \
	rsrcdes.c  \
	typetos.c  \
	stotype.c  \
	timetos.c  \
	stotime.c  \
	attrtos.c  \
	time.c     \
	recnew.c   \
	rsrcnew.c  \
	appiset.c  \
	sortiset.c \
	rsrcmap.c  \
	recmap.c   \
	setname.c  \
	tobuf.c    \
	bufto.c    \
	nrecord.c  \
	strmwrit.c \
	strmread.c \
	errno.c    \
	strerror.c \
	misc.c     \
#	strptime.c \
#

OFILES = $(CFILES:.c=.o)

HFILES = \
	prc.h \
	prcp.h \
	prcsys.h \
#

PAR_CFILES = \
	par.c \
#

PAR_OFILES = $(PAR_CFILES:.c=.o)

PAR_DOCS = par.man par.html

MACSRCS = prc.mcp par.r
WINSRCS = par.dsw par.dsp libprc.dsw libprc.dsp dllmain.c

ALLSRCS = Makefile $(CFILES) $(HFILES) $(PAR_CFILES) $(MACSRCS) $(WINSRCS)
ALLDOCS = $(PAR_DOCS)

ALLFILES = $(ALLSRCS) $(ALLDOCS)
WINFILES = Release/par.exe $(ALLDOCS)

all:	libprc.a par

libprc.a: $(OFILES)
	ar rc libprc.a $(OFILES)
	ranlib libprc.a

par:	$(PAR_OFILES) libprc.a
	$(LD) $(LDFLAGS) -o par $(PAR_OFILES) -L. -lprc

par.html: par.man
	rman --filter HTML --title 'Par(1) Manual Page' par.man > par.html

tarball: prc.tgz

prc.tgz: $(ALLFILES)
	rm -f prc
	mkdir -p prc
	cp $(ALLFILES) prc
	chmod 664 prc/*
	tar cf - prc | gzip > prc.tgz
	rm -rf prc

winball: parwin32.zip

parwin32.zip: $(WINFILES)
	rm -rf par
	mkdir -p par
	cp $(WINFILES) par
	chmod 664 par/*
	zip -r parwin32 par
	rm -rf par

install: libprc.a prc.h par par.man
	@test `whoami` = root || (echo "Must be root to install"; false)
	mkdir -p $(INSTALL_INCDIR)
	mkdir -p $(INSTALL_LIBDIR)
	mkdir -p $(INSTALL_BINDIR)
	mkdir -p $(INSTALL_MANDIR)
	cp prc.h $(INSTALL_INCDIR)/prc.h
	chmod 644 $(INSTALL_INCDIR)/prc.h
	cp libprc.a $(INSTALL_LIBDIR)/libprc.a
	chmod 644 $(INSTALL_LIBDIR)/libprc.a
	cp par $(INSTALL_BINDIR)/par
	chmod 755 $(INSTALL_BINDIR)/par
	cp par.man $(INSTALL_MANDIR)/par.1
	chmod 644 $(INSTALL_MANDIR)/par.1

TAGS:	$(PAR_CFILES) $(CFILES) $(HFILES)
	etags -r '/^prc[^(]*(/' $(PAR_CFILES) $(CFILES) $(HFILES)

clean:
	rm -f *.o *.a par *~
