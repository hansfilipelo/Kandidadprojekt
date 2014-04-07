
	CCC = g++
	CCFLAGS = -g -std=c++11 -pedantic -Wall -Wextra 
	OBJECTS = styrtest.o Abstraction.o MapSection.o Map.o
	BUILDDIR = BUILD

styrtest: $(OBJECTS)
	$(CCC) $(CCFLAGS) styrtest.o -o styrtest

styrtest.o: styrtest.cpp
	$(CCC) $(CCFLAGS) -c styrtest.cpp

Abstraction.o: Abstraction.cpp
	$(CCC) $(CCFLAGS) -c Abstraction.cpp

Map.o: Map.cpp
	$(CCC) $(CCFLAGS) -c Map.cpp

MapSection.o: MapSection.cpp
	$(CCC) $(CCFLAGS) -c MapSection.cpp

clean:
	@ \rm -f ./*.o
	@ \rm -f ./styrtest
