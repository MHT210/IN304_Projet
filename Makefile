CC=gcc
#CFLAGS=-g -O2 -Wall -Werror `sdl2-config --cflags`
CFLAGS=-O2 -Wall `sdl2-config --cflags`
LDLIBS=`sdl2-config --libs` -lm -lSDL2_ttf

# $@ : le nom de la cible
# $^ : les noms des dépendances
# $< : la première dépendance

% : %.c ChainedList.o uvsqgraphics_2.o Button.o ImageFunctions.o Triangles.o CouplesFunctions.o Triangulate.o morphing.o
	$(CC) $(CFLAGS) -o $@ $< ChainedList.o uvsqgraphics_2.o Button.o ImageFunctions.o Triangles.o CouplesFunctions.o Triangulate.o morphing.o $(LDLIBS)

run: main
	./main

valgrind: main
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main

all: demo0 demo1 demo2 demo3 demo4 horloge main
	./demo0
	./demo1
	./demo2
	./demo3
	./demo4
	./horloge
	./main

morphing.o: morphing.c morphing.h
	$(CC) $(CFLAGS) -c morphing.c -o morphing.o

Triangulate.o: Triangulate.c Triangulate.h
	$(CC) $(CFLAGS) -c Triangulate.c -o Triangulate.o

CouplesFunctions.o: CouplesFunctions.c CouplesFunctions.h
	$(CC) $(CFLAGS) -c CouplesFunctions.c -o CouplesFunctions.o

Triangles.o: Triangles.c Triangles.h
	$(CC) $(CFLAGS) -c Triangles.c -o Triangles.o

ImageFunctions.o: ImageFunctions.c ImageFunctions.h
	$(CC) $(CFLAGS) -c ImageFunctions.c -o ImageFunctions.o

Button.o: Button.c Button.h
	$(CC) $(CFLAGS) -c Button.c -o Button.o

ChainedList.o: ChainedList.c ChainedList.h
	$(CC) $(CFLAGS) -c ChainedList.c -o ChainedList.o

uvsqgraphics_2.o: uvsqgraphics_2.c uvsqgraphics_2.h uvsqcouleur_2.h 
	$(CC) $(CFLAGS) -c uvsqgraphics_2.c -o uvsqgraphics_2.o

.PHONY: clean
clean:
	rm -f uvsqgraphics_2.o
	rm -f *.o
	rm -f demo0
	rm -f demo1
	rm -f demo2
	rm -f demo3
	rm -f demo4
	rm -f main
	rm -f horloge
	rm -f morpion


.PHONY: installmac
installmac:
	/bin/bash -c `curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh`
	brew install sdl2
	brew install sdl2_ttf


.PHONY: install
install:
	sudo apt install libsdl2-dev
	sudo apt install libsdl2-ttf


.PHONY: zip
ZIPNAME=IN304_Projet
zip:
	rm -rf ${ZIPNAME}
	mkdir ${ZIPNAME}
	cp demo*.c ${ZIPNAME}
	cp horloge.c ${ZIPNAME}
	cp uvsqgraphics_2.c ${ZIPNAME}
	cp *.h ${ZIPNAME}
	cp doc_uvsqgraphics_SDL2.pdf ${ZIPNAME}
	cp Makefile ${ZIPNAME}
	cp -r POL ${ZIPNAME}
	zip -r ${ZIPNAME}.zip ${ZIPNAME}
#	rm -rf ${ZIPNAME}
	ls -l
