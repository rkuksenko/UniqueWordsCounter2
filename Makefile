gcc := g++ -std=c++20 -O3

all: main.o FileReader.o UniqueWordsCounterAsync.o
	${gcc} -pthread main.o FileReader.o UniqueWordsCounterAsync.o -o UniqueWordsCounter

main.o: main.cpp
	${gcc} -c main.cpp -o main.o

FileReader.o: FileReader.cpp
	${gcc} -c FileReader.cpp -o FileReader.o

UniqueWordsCounterAsync.o: UniqueWordsCounterAsync.cpp
	${gcc} -c UniqueWordsCounterAsync.cpp -o UniqueWordsCounterAsync.o

clean:
	rm -f *.o UniqueWordsCounter

.PHONY:
	all clean
