##
# Makefile for dust_engine
# Connor Ennis, 2022
##

# Windows SDL2 Library directory
DIR_WINLIBS = ./../../winlibs
SRCS = src/*
INCS = include
PROG_NAME = dust_demo

linux: builddir
	gcc -I$(INCS) $(SRCS) \
	-lm -lSDL2 -lSDL2main -O3 -o build/linux/$(PROG_NAME)

linux_debug: debugdir
	gcc -Wall -I$(INCS) $(SRCS) \
	-lm -lSDL2 -lSDL2main -g -o debug/linux/$(PROG_NAME)

windows: builddir
	x86_64-w64-mingw32-gcc -I$(INCS) -I$(DIR_WINLIBS)/SDL2/x86_64-w64-mingw32/include \
	$(SRCS) \
	-L$(DIR_WINLIBS)/SDL2/x86_64-w64-mingw32/lib -lm -lmingw32 -lSDL2main -lSDL2 \
	-O3 -m64 -mwindows -o build/windows/$(PROG_NAME).exe

windows_debug: debugdir
	x86_64-w64-mingw32-gcc -I$(INCS) -I$(DIR_WINLIBS)/SDL2/x86_64-w64-mingw32/include \
	$(SRCS) \
	-L$(DIR_WINLIBS)/SDL2/x86_64-w64-mingw32/lib -lm -lmingw32 -lSDL2main -lSDL2 \
	-Wall -g -m64 -mwindows -o debug/windows/$(PROG_NAME).exe


builddir:
	mkdir -p build/linux
	mkdir -p build/windows

debugdir:
	mkdir -p debug/linux
	mkdir -p debug/windows

run_linux:
	./build/linux/$(PROG_NAME)
run_linux_debug:
	./debug/linux/$(PROG_NAME)

run_windows:
	./build/windows/$(PROG_NAME).exe

run_windows_debug:
	./debug/windows/$(PROG_NAME).exe

clean:
	rm build/linux/$(PROG_NAME)
	rm build/windows/$(PROG_NAME).exe
