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

maolan-cli:
	$(MAKE) $(MFLAGS) -C src maolan-cli
