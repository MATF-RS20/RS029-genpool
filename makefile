CC= g++
FLAGS= -std=c++17 -pg -Wall -Wextra
BIN= BestRsProject

SRC= $(shell find src -type f -name *.cpp)
OBJECT= $(patsubst src/%,build/%,$(SRC:.cpp=.o))

default: objects bin run

objects: $(SRC)

src/%.cpp:
	$(CC) $(FLAGS) -o build/$* -c $@

bin: $(OBJECT)
	$(CC) $(FLAGS) -o bin/$(BIN) $<

build/%.o: src/%.cpp
	$(CC) $(FLAGS) -o build/$*.o -c $^

run:
	./bin/$(BIN)

.PHONY: clean

clean:
	rm build/*
	rm bin/*