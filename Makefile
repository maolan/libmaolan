PREFIX ?= /usr/local

all:
	$(MAKE) $(MFLAGS) -C src all

clean:
	$(MAKE) $(MFLAGS) -C src clean

distclean:
	rm -rf \
		configure \
		autom4te.cache \
		config.log \
		config.status \
		makeinclude
	$(MAKE) $(MFLAGS) -C src distclean


git: distclean
	autoconf
	env CC=$(CC) CXX=$(CXX) CPPFLAGS="$(CPPFLAGS)" LDFLAGS="$(LDFLAGS)" ./configure
	$(MAKE) $(MFLAGS) -C src gendep
	rm -rf autom4te.cache config.log config.status

install:
	install -m 0755 src/libmaolan.so ${DESTDIR}${PREFIX}/lib
	cp -r maolan ${DESTDIR}${PREFIX}/include
