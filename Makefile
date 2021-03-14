VigenereChpher: DuplicateStringFinder.o
	g++ -Wall -std=c++17 -o DuplicateStringFinder DuplicateStringFinder.o

DuplicateStringFinder.o: DuplicateStringFinder.cpp
	g++ -std=c++17 -c DuplicateStringFinder.cpp

clean:
	rm *.o *DuplicateStringFinder
	