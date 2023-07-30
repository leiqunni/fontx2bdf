#

# $B%3%s%Q%$%i$N@_Dj(B
CC=gcc
#CC=tcc
#CC=lcc
#CC=cl

# $B%*%V%8%'%/%H%U%!%$%k$N3HD%;R(B
O=o
#O=obj

# $B<B9T%U%!%$%k@8@.%*%W%7%g%s(B
EXEC=-o $(PROG)
#EXEC=-e$(PROG)

# $B%$%s%9%H!<%k@h;XDj(B
PREFIX=/usr/local
BINDIR=$(PREFIX)/bin
MANDIR=$(PREFIX)/man/man1

#
CFLAGS=

#
PROG=fontx2bdf
OBJS=fontx2bdf.$(O)
MAN=fontx2bdf.1


all: $(PROG)

install: all
	install -s -c $(PROG) $(BINDIR)

install.man:
	install -c $(MAN) $(MANDIR)
	gzip -9n $(MANDIR)/$(MAN)
	
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(EXEC) $(OBJS)

$(OBJS): makefile

.c.$(O):
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS)
#	del *.$(OBJS)
