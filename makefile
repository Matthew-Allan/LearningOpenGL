srcdir := "$(dir $(abspath $(lastword $(MAKEFILE_LIST))))"
files := main glad/glad shader app buffers
outputnm := main.exe
libs := mingw32 SDL2main SDL2
gccBase := -o $(srcdir)$(outputnm) $(patsubst %,${srcdir}src/%.c,$(files)) -I$(srcdir)include -L$(srcdir)lib $(patsubst %,-l%,$(libs))


all:
	gcc $(gccBase)
	$(srcdir)main.exe
build-console:
	gcc $(gccBase)
build:
	gcc $(gccBase) -mwindows
run:
	$(srcdir)main.exe
test-gcc-base:
	$(gccBase)