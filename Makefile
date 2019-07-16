PROG_NAME = main
CC = g++
FLAGS = -std=c++17 -lglfw3 -lGL -lglut -lGLEW -lm -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread -lSOIL
SOURCES = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp, build/%.o, $(SOURCES))
DEPENDS = $(patsubst src/%.cpp, .depend/%.d, $(SOURCES))

all: run

$(PROG_NAME): $(OBJS)
	$(CC) $(OBJS) -o $(PROG_NAME) $(FLAGS)

run: $(PROG_NAME)
	./$(PROG_NAME)

-include $(DEPENDS)

.depend:
	mkdir -p .depend

.depend/%.d: src/%.cpp .depend Makefile
	$(CC) $(FLAGS) -MM -MT '$(patsubst src/%.cpp,build/%.o,$<)' $< -MF $@

build/%.o: src/%.cpp .depend/%.d
	$(CC) $(FLAGS) -c $< -o $@ 