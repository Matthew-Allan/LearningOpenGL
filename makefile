srcdir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
files := main shader app buffers image texture files transforms object world camera input glad/glad 
outputnm := main
unixLibs := SDL2 SDL2_image m
windowsLibs := mingw32 SDL2main SDL2 SDL2_image
gccBase := $(patsubst %,"${srcdir}src/%.c",$(files)) -I"$(srcdir)include" -L"$(srcdir)lib"

ifeq ($(OS), Windows_NT)
	gccCommand := gcc -o "$(srcdir)$(outputnm).exe" $(gccBase) $(patsubst %,-l%,$(windowsLibs))
	extention := .exe
else
	osName := $(shell uname -s)
	ifeq ($(osName),Linux)
		gccCommand := gcc -o "$(srcdir)$(outputnm)" $(gccBase) $(patsubst %,-l%,$(unixLibs))
		extention := 
	endif
	ifeq ($(osName),Darwin)
		gccCommand := gcc -o "$(srcdir)$(outputnm).app" $(gccBase) $(patsubst %,-l%,$(unixLibs))
		extention := .app
	endif
endif

runCommand := "$(srcdir)$(outputnm)$(extention)"

all:
	$(gccCommand)
	$(runCommand)
build:
	$(gccCommand)
build-win-nocons:
	$(gccCommand) -mwindows
move-export:
	copy "$(srcdir)$(outputnm)$(extention)" "$(srcdir)export/$(outputnm)$(extention)"
	xcopy "$(srcdir)img" "$(srcdir)export/img" /E /I
	xcopy "$(srcdir)shaders" "$(srcdir)export/shaders" /E /I
debug:
	$(gccCommand) -g
run:
	$(runCommand)