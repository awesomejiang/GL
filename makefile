CXX = g++
FLAGS = -std=c++11

PROGS = defer theresa

SUBS = object scene camera gbuffer shadow shader framebuffer model mesh texture

LIBS = $(patsubst %, lib/lib%.so, $(SUBS))
LINKS = $(patsubst %, -l%, $(SUBS))
LINKS +=  -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXinerama -lXxf86vm -lXcursor -lXi -ldl -lassimp

DIRS = lib obj

all: $(DIRS) $(PROGS)

$(PROGS): %: obj/%.o $(LIBS)
	$(CXX) $(FLAGS) -Wl,-rpath lib -Iinclude $< src/glad.c -o $@ -Llib $(LINKS)

$(LIBS): lib/lib%.so: src/%.cpp include/%.h
	$(CXX) $(FLAGS) -fPIC -shared -Iinclude -o $@ $<

obj/%.o: %.cpp
	$(CXX) $(FLAGS) -c -Iinclude -o $@ $<

$(DIRS):
	mkdir -p $@

clean:
	rm -rf $(PROGS) $(DIRS)