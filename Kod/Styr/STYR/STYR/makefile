
	CCC = g++
	CCFLAGS = -g -std=c++11 -pedantic -Wall -Wextra 
	OBJECTS = $(BUILDDIR)/styrtest.o $(BUILDDIR)/Abstraction.o $(BUILDDIR)/MapSection.o $(BUILDDIR)/Map.o
	BUILDDIR = BUILD

$(BUILDDIR)/styrtest: $(OBJECTS)
	$(CCC) $(CCFLAGS) $(BUILDDIR)/styrtest.o $(BUILDDIR)/styrtest

$(BUILDDIR)/styrtest.o: styrtest.cpp
	$(CCC) $(CCFLAGS) -c styrtest.cpp

$(BUILDDIR)/Abstraction.o: Abstraction.cpp
	$(CCC) $(CCFLAGS) -c Abstraction.cpp

$(BUILDDIR)/Map.o: Map.cpp
	$(CCC) $(CCFLAGS) -c Map.cpp

$(BUILDDIR)/MapSection.o: MapSection.cpp
	$(CCC) $(CCFLAGS) -c MapSection.cpp

clean:
	@ \rm -f ./$(BUILDDIR)/*.o
	@ \rm -f ./$(BUILDDIR)/styrtest
