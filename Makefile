PROG_NAME = main
CC = g++
FLAGS = -std=c++17 -lglfw3 -lGL -lglut -lGLEW -lm -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread -lSOIL
SOURCES = $(wildcard src/*.cpp)
OBJS = $(patsubst %.cpp, build/%.o, $(notdir $(SOURCES)))
DEPENDS = $(patsubst build/%.o, .depend/%.d, $(OBJS))

.PHONY: all clean
all: run
clean:
	rm -r .depend build
$(PROG_NAME): Makefile $(OBJS)
	$(CC) $(OBJS) -o $(PROG_NAME) $(FLAGS)

run: $(PROG_NAME)
	./$(PROG_NAME)

-include $(DEPENDS)
	

.depend/%.d: src/%.cpp .depend
	$(CC) $(FLAGS) -MM -MT '$(patsubst src/%.cpp,build/%.o,$<)' $< -MF $@
build/%.o: src/%.cpp .depend/%.d
	$(CC) $(FLAGS) -c $< -o $@