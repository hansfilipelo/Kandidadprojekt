
	CCC = g++
	CCFLAGS = -g -std=c++11 -pedantic -Wall -Wextra 
	OBJECTS = styrtest.cpp
	BUILDDIR = BUILD

styrtest: $(OBJECTS)
	$(CCC) $(CCFLAGS) $(BUILDDIR)/styrtest

$(BUILDDIR)/styrtest.o: styrtest.cpp
	$(CCC) $(CCFLAGS) -c styrtest.cpp.cc 

$(BUILDDIR)/Abstraction.o: Abstraction.cpp
	$(CCC) $(CCFLAGS) -c Abstraction.cpp

$(BUILDDIR)/Map.o: Map.cpp
	$(CCC) $(CCFLAGS) -c Map.cpp

$(BUILDDIR)/MapSection.o: MapSection.cpp
	$(CCC) $(CCFLAGS) -c MapSection.cpp

clean:
	@ \rm -f ./$(BUILDDIR)/*.o
	@ \rm -f ./$(BUILDDIR)/styrtest
