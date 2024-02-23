srcdir := "$(dir $(abspath $(lastword $(MAKEFILE_LIST))))"
files := main shader app buffers image texture glad/glad 
outputnm := main
unixLibs := SDL2 SDL2_image
windowsLibs := mingw32 SDL2main SDL2 SDL2_image
gccBase := $(patsubst %,${srcdir}src/%.c,$(files)) -I$(srcdir)include -L$(srcdir)lib
unixGccBase := -o $(srcdir)$(outputnm) $(gccBase) $(patsubst %,-l%,$(unixLibs))
macGccBase := -o $(srcdir)$(outputnm).app $(gccBase) $(patsubst %,-l%,$(unixLibs))
mingwGccBase := -o $(srcdir)$(outputnm).exe $(gccBase) $(patsubst %,-l%,$(windowsLibs))


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
build-mac:
	gcc $(macGccBase)
run-windows:
	$(srcdir)main.exe
run-unix:
	$(srcdir)main
run-mac:
	$(srcdir)main.app
test-gcc-base:
	$(gccBase)