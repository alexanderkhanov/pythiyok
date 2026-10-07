SRC=mycode.cpp

%.o: %.cpp
	$(CXX) -I $(PYTHIA_PATH)/include $(shell root-config --cflags) -c $<

all: $(SRC:.cpp=.o)
	$(CXX) $(SRC:.cpp=.o) -L $(PYTHIA_PATH)/lib -lpythia8 $(shell root-config --libs) -o run_mycode
