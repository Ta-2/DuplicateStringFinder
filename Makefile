VigenereChpher: DuplicateStringFinder.o
	g++ -Wall -std=c++17 -o DuplicateStringFinder \
	DuplicateStringFinder.o ./../StatisticsCalculator/StatisticsCalculatorSource.o \
	-L ./../StatisticsCalculator

DuplicateStringFinder.o: DuplicateStringFinder.cpp
	g++ -std=c++17 \
	-I ./../StatisticsCalculator \
	 -c DuplicateStringFinder.cpp

clean:
	rm *.o *DuplicateStringFinder
	