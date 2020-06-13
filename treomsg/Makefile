

GLIB_CFLAGS:=$(shell pkg-config --cflags glib-2.0)
GLIB_LIBS:=$(shell pkg-config --libs glib-2.0)

CFLAGS=-g
CFLAGS+=$(GLIB_CFLAGS)

dumpmsg: dumpmsg.o
	$(CC) -o $@ dumpmsg.o $(GLIB_LIBS)

clean:
	rm -f *~ *.o core* dumpmsg

