CXX = g++
OPTIONS = -Wall
PROGRAM = main.out
FILES = main.cpp Student.cpp Course.cpp RecommendationSystem.cpp

all: $(PROGRAM)

run: $(PROGRAM)
	./$(PROGRAM)

$(PROGRAM): $(FILES)
	$(CXX) $(OPTIONS) $(FILES) -o $(PROGRAM) 

clean:
	rm -f $(PROGRAM)
