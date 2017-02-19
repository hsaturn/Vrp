# Quick makefile, sry

TARGETS = build rot snowman 2d basic

all: $(TARGETS)

clean:
	rm -rf $(TARGETS)

INCLUDE=
LIBS=-lGL -lglfw -lGLEW -lSOIL -lGLU -lglut

GXX=g++ $(INCLUDE)

.PHONY:
build:
	mkdir build

rot: build/rot.o
	$(GXX) $< $(LIBS) -o $@

snowman: build/snowman.o
	$(GXX) $< $(LIBS) -o $@

basic: build/basic.o
	$(GXX) $< $(LIBS) -o $@

2d: build/2d.o
	$(GXX) $< $(LIBS) -o $@


build/%.o: %.cpp
	$(GXX) -c "$<" -o "$@"

build/%.o: %.c
	$(GXX) -c "$<" -o "$@"

