CC := g++ --std=c++11

CCFLAGS += -l SDL2 -l SDL2_image -l SDL2_ttf -l SDL2_mixer



run: game.out
	./game.out 


game.out: main.o rsdl.o brsdl.o components.o stage.o 
	$(CC) build/main.o build/rsdl.o build/brsdl.o build/components.o build/stage.o $(CCFLAGS) -o game.out


main.o: code/main.cpp  liberaries/rsdl.hpp code/stage.hpp code/components.hpp code/game_consts.hpp code/brsdl.hpp
	$(CC) -c code/main.cpp -o build/main.o

brsdl.o: code/brsdl.cpp code/brsdl.hpp code/game_consts.hpp
	$(CC) -c code/brsdl.cpp -o build/brsdl.o

components.o: code/components.cpp code/components.hpp code/game_consts.hpp code/brsdl.hpp
	$(CC) -c code/components.cpp -o build/components.o

stage.o: code/stage.cpp code/stage.hpp code/components.hpp code/game_consts.hpp code/brsdl.hpp
	$(CC) -c code/stage.cpp -o build/stage.o

	
	
rsdl.o: liberaries/rsdl.hpp liberaries/rsdl.cpp
	$(CC) -c liberaries/rsdl.cpp -o build/rsdl.o


clean:
	rm -r *.o 