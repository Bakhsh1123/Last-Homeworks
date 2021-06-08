CC := g++ --std=c++11

permitMaker: main.o documentory.o documents.o organs.o CSV.o
	$(CC) build/main.o build/documentory.o build/documents.o build/organs.o build/CSV.o -o permitMaker

main.o: code/main.cpp code/documentory.hpp
	$(CC) -c code/main.cpp -o build/main.o

documentory.o: code/documentory.cpp code/documentory.hpp code/organs.hpp code/documents.hpp code/CSV.hpp
	$(CC) -c code/documentory.cpp -o build/documentory.o

documents.o: code/documents.cpp code/documents.hpp code/organs.hpp
	$(CC) -c code/documents.cpp -o build/documents.o
	
CSV.o: code/CSV.cpp code/CSV.hpp
	$(CC) -c code/CSV.cpp -o build/CSV.o

organs.o: code/organs.cpp code/organs.hpp
	$(CC) -c code/organs.cpp -o build/organs.o







