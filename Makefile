PROG_NAME = main
CC = g++
FLAGS = -std=c++17 -lglfw3 -lGL -lglut -lGLEW -lm -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread -lSOIL
SOURCEDIR = src
SOURCES = $(shell find $(SOURCEDIR) -name '*.cpp')
OBJS = $(patsubst src/%.cpp, .obj/%.o, $(SOURCES))
DEPENDS = $(patsubst .obj/%.o, .depend/%.d, $(OBJS))

.PHONY: all clean
all: run
clean:
	rm -r .depend .obj
$(PROG_NAME): Makefile $(OBJS)
	$(CC) $(OBJS) -o $(PROG_NAME) $(FLAGS)

run: $(PROG_NAME)
	./$(PROG_NAME)

-include $(DEPENDS)
	

.depend/%.d: src/%.h .depend
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) -MM -MT '$(patsubst src/%.h,build/%.o,$<)' $(patsubst %.h,%.cpp,$<) -MF $@
.depend/main.d: src/main.cpp
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) -MM -MT '$(patsubst src/%.cpp,build/%.o,$<)' $< -MF $@
.obj/%.o: src/%.cpp .depend/%.d
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@