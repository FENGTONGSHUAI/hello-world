CFLAGS?=	-Wall -ggdb -W -O
CC?=		gcc
LIBS?=
LDFLAGS?=
PREFIX?=	/usr/local
VERSION=1.5
TMPDIR=/tmp/webbench-$(VERSION)

all:   webbench

install: webbench
	# 将webbench删除symbol table并复制到$(DESTDIR)$(PREFIX)/bin
	install -s webbench $(DESTDIR)$(PREFIX)/bin
#	# 指定文件权限为644并复制到$(DESTDIR)$(PREFIX)/man/man1
#	install -m 644 webbench.1 $(DESTDIR)$(PREFIX)/man/man1	
#	# 创建$(DESTDIR)$(PREFIX)/share/doc/webbench 目录
#	install -d $(DESTDIR)$(PREFIX)/share/doc/webbench
#	# 复制文件到$(DESTDIR)$(PREFIX)/share/doc/webbench 目录中
#	install -m 644 debian/copyright $(DESTDIR)$(PREFIX)/share/doc/webbench
#	install -m 644 debian/changelog $(DESTDIR)$(PREFIX)/share/doc/webbench

webbench: webbench.o Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) -o webbench webbench.o $(LIBS) 

clean:
# 前面的 - 用于表示忽略错误https://dev59.com/onA75IYBdhLWcg3w_umD
	-rm -f *.o webbench *~ core *.core tags 
# 隐含规则, 参考https://seisman.github.io/how-to-write-makefile/implicit_rules.html
webbench.o:	webbench.c socket.c Makefile

tar:   clean
	rm -rf $(TMPDIR)
	install -d $(TMPDIR)
	cp -p Makefile webbench.c socket.c webbench.1 $(TMPDIR)
	-cd $(TMPDIR) && cd .. && tar cozf webbench-$(VERSION).tar.gz webbench-$(VERSION)