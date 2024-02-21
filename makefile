srcdir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
files := ${srcdir}/src/main.c $(srcdir)/src/glad.c $(srcdir)/src/shader.c
outputnm := main.exe

all:
	gcc $(files) -I$(srcdir)/include/ -L$(srcdir)/lib -lmingw32 -lSDL2main -lSDL2 -o $(srcdir)/$(outputnm)"
	$(srcdir)/main.exe
build:
	gcc ${srcdir}/src/main.c $(srcdir)/src/glad.c -I$(srcdir)/include/ -L$(srcdir)/lib -lmingw32 -lSDL2main -lSDL2 -o $(srcdir)/main.exe"
run:
	$(srcdir)/main.exe