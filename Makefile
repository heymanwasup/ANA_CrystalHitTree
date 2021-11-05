CXX       := g++
INCLUDES  := $(shell root-config --cflags)
LIBS  := $(shell root-config --glibs) 
OBJ = main_readCrystalHits.o crystalHits.o SimpleHistSVC.o
INC = crystalHits.h SimpleHistSVC.h

%.o: %.C $(INC)
	$(CXX) $(INCLUDES)  -c -o $@ $< $(LIBS)

ana_readcrystalHits : $(OBJ)
	$(CXX) -o $@ $^ $(INCLUDES) $(LIBS)
