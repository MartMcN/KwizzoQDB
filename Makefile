#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need SDL2 (http://www.libsdl.org):
# Linux:
#   apt-get install libsdl2-dev
# Mac OS X:
#   brew install sdl2
# MSYS2:
#   pacman -S mingw-w64-i686-SDL
#

#CXX = g++
#CXX = clang++

EXE = KwizzoQDB
SOURCES =  ./kwizzo_main.cpp ./kwizzo.cpp ./kwizzo_ui.cpp ./kwizzo_xml.cpp
SOURCES += ./imgui-master/imgui.cpp ./imgui-master/imgui_demo.cpp ./imgui-master/imgui_draw.cpp ./imgui-master/imgui_widgets.cpp
SOURCES += ./imgui-master/examples/imgui_impl_sdl.cpp ./imgui-master/examples/imgui_impl_opengl2.cpp
SOURCES += ./tinyxml2.cpp

OBJS = $(addprefix bin/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
UNAME_S := $(shell uname -s)

CXXFLAGS = -I./ -I./imgui-master/ -I./imgui-master/examples/
CXXFLAGS += -g -Wall -Wformat
LIBS =

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL -ldl `sdl2-config --libs`

	CXXFLAGS += `sdl2-config --cflags`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo `sdl2-config --libs`
	LIBS += -L/usr/local/lib -L/opt/local/lib

	CXXFLAGS += `sdl2-config --cflags`
	CXXFLAGS += -I/usr/local/include -I/opt/local/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(findstring MINGW,$(UNAME_S)),MINGW)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lgdi32 -lopengl32 -limm32 `pkg-config --static --libs sdl2`

	CXXFLAGS += `pkg-config --cflags sdl2`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

bin/%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

bin/%.o:./imgui-master/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

bin/%.o:./imgui-master/examples/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
