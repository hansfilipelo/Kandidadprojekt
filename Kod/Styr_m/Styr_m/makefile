
	CCC = g++
	CCFLAGS = -g -std=c++11 -pedantic -Wall -Wextra 
	OBJECTS = Map.o MapSection.o Abstraction.o styrtest.o slave.o Communication.o
	BUILDDIR = BUILD

styrtest: $(OBJECTS)
	$(CCC) $(CCFLAGS) $(OBJECTS) -o styrtest

styrtest.o: styrtest.cpp Abstraction.o Communication.o
	$(CCC) $(CCFLAGS) -c styrtest.cpp

Communication.o: Communication.cpp
	$(CCC) $(CCFLAGS) -c Communication.cpp

Abstraction.o: Abstraction.cpp MapSection.o
	$(CCC) $(CCFLAGS) -c Abstraction.cpp

Map.o: Map.cpp
	$(CCC) $(CCFLAGS) -c Map.cpp

MapSection.o: MapSection.cpp Map.o
	$(CCC) $(CCFLAGS) -c MapSection.cpp

slave.o: ../../../sensormodul/sensormodul/slave.cpp
	$(CCC) $(CCFLAGS) -c ../../../sensormodul/sensormodul/slave.cpp

clean:
	@ \rm -f ./*.o
	@ \rm -f ./styrtest
