CFLAGS = -O2
CXXFLAGS = $(CFLAGS) -std=c++14
LDFLAGS = -L/usr/local/lib -pthread -lsndfile
CXXFILES = \
	audiofile.cxx \
	maolan.cxx \
	th.cxx
OBJECTS = $(CXXFILES:.cxx=.o) $(CFILES:.c=.o)
-include makeinclude

.MAIN: all

all: maolan

maolan: $(OBJECTS)
	c++ $(OBJECTS) $(LDFLAGS) -o maolan

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf $(OBJECTS) maolan
