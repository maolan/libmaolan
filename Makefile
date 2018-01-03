CFLAGS = $(CPPFLAGS) -O2
CXXFLAGS = $(CFLAGS) -std=c++14
LDFLAGS = -pthread -lsndfile
CXXFILES = \
	audiofile.cxx \
	audioengine.cxx \
	audiooss.cxx \
	maolan.cxx \
	th.cxx
OBJECTS = $(CXXFILES:.cxx=.o) $(CFILES:.c=.o)
-include makeinclude

.MAIN: all

all: maolan

maolan: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o maolan

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS) maolan
