#

# コンパイラの設定
CC=gcc
#CC=tcc
#CC=lcc
#CC=cl

# オブジェクトファイルの拡張子
O=o
#O=obj

# 実行ファイル生成オプション
EXEC=-o $(PROG)
#EXEC=-e$(PROG)

# インストール先指定
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
