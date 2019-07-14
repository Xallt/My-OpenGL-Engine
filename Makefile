PROG_NAME = main
FLAGS = -std=c++17 -lglfw3 -lGL -lglut -lGLEW -lm -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread -lSOIL
SOURCES = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp, build/%.o, $(SOURCES))

all: run

build/%.o: src/%.cpp
	g++ -c $< -o $@ $(FLAGS)
$(PROG_NAME): $(OBJS)
	g++ $(OBJS) -o $(PROG_NAME) $(FLAGS)

run: $(PROG_NAME)
	./$(PROG_NAME)