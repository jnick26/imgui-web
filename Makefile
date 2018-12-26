#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install libglfw-dev
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
#

#CXX = g++
#CXX = clang++

CXX = emcc
OUTPUT = imgui.html

SOURCES = main.cpp
SOURCES += imgui_impl_glfw.cpp imgui_impl_opengl3.cpp
SOURCES += imgui.cpp imgui_draw.cpp imgui_demo.cpp imgui_freetype.cpp
#SOURCES += ../../imgui.cpp ../../imgui_demo.cpp ../../imgui_draw.cpp
#SOURCES += ../libs/gl3w/GL/gl3w.c
OPTS = -O2 --llvm-opts 2 

UNAME_S := $(shell uname -s)


LIBS = -lGLU -lGL


all: $(SOURCES) $(OUTPUT) 

$(OUTPUT): $(SOURCES)
	$(CXX)  $(SOURCES) -std=c++11 --shell-file frame.html -o $(OUTPUT) $(LIBS) -s MODULARIZE=1 -s WASM=0 -s USE_FREETYPE=1 -s USE_WEBGL2=1 -s USE_GLFW=3  -s FULL_ES3=1 --preload-file data

clean:
	rm -f $(OUTPUT)
