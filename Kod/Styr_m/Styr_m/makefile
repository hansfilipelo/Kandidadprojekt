
	CCC = g++
	CCFLAGS = -g -std=c++11 -pedantic -Wall -Wextra 
	OBJECTS = node.o Map.o MapSection.o Abstraction.o Styr_m.o slave.o Communication.o
	BUILDDIR = BUILD

Styr_m: $(OBJECTS)
	$(CCC) $(CCFLAGS) $(OBJECTS) -o Styr_m

Styr_m.o: Styr_m.cpp Abstraction.o Communication.o
	$(CCC) $(CCFLAGS) -c Styr_m.cpp

Communication.o: Communication.cpp
	$(CCC) $(CCFLAGS) -c Communication.cpp

Abstraction.o: Abstraction.cpp MapSection.o
	$(CCC) $(CCFLAGS) -c Abstraction.cpp

Map.o: Map.cpp
	$(CCC) $(CCFLAGS) -c Map.cpp

MapSection.o: MapSection.cpp Map.o
	$(CCC) $(CCFLAGS) -c MapSection.cpp

slave.o: slave.cpp
	$(CCC) $(CCFLAGS) -c slave.cpp

node.o: node.cpp
	$(CCC) $(CCFLAGS) -c node.cpp

clean:
	@ \rm -f ./*.o
	@ \rm -f ./Styr_m
