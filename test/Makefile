CPP=g++

PYTHIA_PATH=/home/khanov/work/mg5/MG5_aMC_v3_7_1/HEPTools/pythia8

SRC=generate_dataset.cpp

%.o: %.cpp
	$(CPP) -I $(PYTHIA_PATH)/include $(shell root-config --cflags) -c $<

all: $(SRC:.cpp=.o)
	$(CPP) $(SRC:.cpp=.o) -L $(PYTHIA_PATH)/lib -lpythia8lhapdf6 -lpythia8 $(shell root-config --libs) -o run_dataset
