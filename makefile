CC= g++
FLAGS= -std=c++17 -pg -Wall -Wextra -Iinclude -DLOG
LINKS= -lGL -lGLU -lglut `libpng-config --ldflags --libs` -lm
BIN= BestRsProject

HEADS=$(shell find include -type f -name *.hpp)
SRC= $(shell find src -type f -name *.cpp)
OBJECT= $(patsubst src/%,build/%,$(SRC:.cpp=.o))

default: objects bin run

objects: $(SRC)

src/%.cpp:
	$(CC) $(FLAGS) -o build/$* -c $@

bin: $(OBJECT)
	$(CC) $(FLAGS) -o bin/$(BIN) $^ $(LINKS)

build/%.o: src/%.cpp
	$(CC) $(FLAGS) -o build/$*.o -c $^

run:
	./bin/$(BIN)

.PHONY: clean

clean:
	rm build/*
	rm bin/*
	rm gmon.out
