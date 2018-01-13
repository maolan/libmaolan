CFLAGS = -I.. $(CPPFLAGS) -O2
CXXFLAGS = $(CFLAGS) -std=c++14
LDFLAGS = -pthread -lsndfile
CXXFILES = \
	audioconnection.cxx \
	audiochunk.cxx \
	audiofileinput.cxx \
	audioio.cxx \
	audioossout.cxx \
	maolan.cxx
OBJECTS = $(CXXFILES:.cxx=.o) $(CFILES:.c=.o)
-include ../makeinclude

.MAIN: all

all: maolan makedepend

-include makedepend

maolan: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o maolan

clean:
	rm -rf $(OBJECTS) maolan

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

distclean: clean
	rm -rf makedepend

makedepend:
	touch makedepend

gendep: $(CXXFILES)
	$(CXX) -MM -DMAKEDEPEND $(CXXFLAGS) $(CXXFILES) >makedepend
