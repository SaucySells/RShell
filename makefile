FLAGS = -Wall -Werror -ansi -pedantic
EXEC = bin/rshell
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:.cpp = .o)

$(EXEC): $(OBJECTS)
	mkdir -p bin
	g++ $(OBJECTS) -o $(EXEC)
	
%.o: %.cpp
	g++ -c $(FLAGS) $c -o $@

clean:
	rm -r ./bin