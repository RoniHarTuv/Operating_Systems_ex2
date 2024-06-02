CXX = g++
CXXFLAGS = -g -Wall -fprofile-arcs -ftest-coverage

.PHONY: all clean

all: ttt mync

ttt: ttt.o
	$(CXX) $(CXXFLAGS) $^ -o $@

mync: mync.o
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -lgcov -c $< -o $@

clean:
	rm -f *.o ttt mync