srcdir := "$(dir $(abspath $(lastword $(MAKEFILE_LIST))))"
files := main glad/glad shader app buffers
outputnm := main.exe
windowsLibs := mingw32 SDL2main SDL2
unixLibs := SDL2
gccBase := -o $(srcdir)$(outputnm) $(patsubst %,${srcdir}src/%.c,$(files)) -I$(srcdir)include -L$(srcdir)lib
unixGccBase := $(gccBase) $(patsubst %,-l%,$(unixLibs))
mingwGccBase := $(gccBase) $(patsubst %,-l%,$(windowsLibs))


all:
	gcc $(gccBase)
	$(srcdir)main.exe
build-console:
	gcc $(mingwGccBase)
build-windows:
	gcc $(mingwGccBase) -mwindows
build-terminal:
	gcc $(unixGccBase)
build-unix:
	gcc $(unixGccBase)
run:
	$(srcdir)main.exe
test-gcc-base:
	$(gccBase)