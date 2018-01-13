CFLAGS = -I.. $(CPPFLAGS) -O2 -fPIC
CXXFLAGS = $(CFLAGS) -std=c++14
LDFLAGS = -pthread -lsndfile
LDFLAGS_SHARED = -shared
CXXFILES = \
	audiochunk.cxx \
	audioconnection.cxx \
	audiofileinput.cxx \
	audioinput.cxx \
	audioio.cxx \
	audioossout.cxx \
	config.cxx
OBJECTS = $(CXXFILES:.cxx=.o) $(CFILES:.c=.o)
-include ../makeinclude

.MAIN: all

all: libmaolan.so makedepend

-include makedepend

libmaolan.so: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(LDFLAGS_SHARED) -Wl,-soname,libmaolan.so -o libmaolan.so

clean:
	rm -rf $(OBJECTS) libmaolan.so maolan-cli

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

distclean: clean
	rm -rf makedepend

makedepend:
	touch makedepend

gendep: $(CXXFILES)
	$(CXX) -MM -DMAKEDEPEND $(CXXFLAGS) $(CXXFILES) >makedepend

maolan-cli: maolancli.cxx
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -L. -lmaolan -o $@ maolancli.cxx
