CFLAGS =  -Iinclude $(CPPFLAGS) -O2
CXXFLAGS = $(CFLAGS) -std=c++14
LDFLAGS = -pthread -lsndfile
CXXFILES = \
	audiofile.cxx \
	audioengine.cxx \
	audiooss.cxx \
	maolan.cxx
OBJECTS = $(CXXFILES:.cxx=.o) $(CFILES:.c=.o)
-include makeinclude

.MAIN: all

all: maolan

-include makedepend

maolan: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o maolan

clean:
	rm -rf $(OBJECTS) maolan

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

distclean: clean
	rm -rf configure autom4te.cache config.log config.status makeinclude

gendep:
	$(CXX) -MM -DMAKEDEPEND $(CXXFLAGS) $(CXXFILES) >makedepend
