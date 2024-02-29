srcdir := "$(dir $(abspath $(lastword $(MAKEFILE_LIST))))"
files := main shader app buffers image texture files transforms glad/glad 
outputnm := main
unixLibs := SDL2 SDL2_image
windowsLibs := mingw32 SDL2main SDL2 SDL2_image
gccBase := $(patsubst %,${srcdir}src/%.c,$(files)) -I$(srcdir)include -L$(srcdir)lib

ifeq ($(OS), Windows_NT)
	gccCommand := gcc -o $(srcdir)$(outputnm).exe $(gccBase) $(patsubst %,-l%,$(windowsLibs))
	runCommand := $(srcdir)main.exe
else
	osName := $(shell uname -s)
	ifeq ($(osName),Linux)
		gccCommand := gcc -o $(srcdir)$(outputnm) $(gccBase) $(patsubst %,-l%,$(unixLibs))
		runCommand := $(srcdir)main
	endif
	ifeq ($(osName),Darwin)
		gccCommand := gcc -o $(srcdir)$(outputnm).app $(gccBase) $(patsubst %,-l%,$(unixLibs))
		runCommand := $(srcdir)main.app
	endif
endif

all:
	$(gccCommand)
	$(runCommand)
build:
	$(gccCommand)
build-win-nocons:
	$(gccCommand) -mwindows
run:
	$(runCommand)