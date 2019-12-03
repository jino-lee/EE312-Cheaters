plagiarismCatcher: plagiarismCatcher.o hashTable.o
	g++ -o plagiarismCatcher plagiarismCatcher.o hashTable.o
main: plagiarismCatcher.cpp hashTable.h
	g++ plagiarismCatcher.cpp
HashTable.o: hashTable.cpp hashTable.h
	g++ hashTable.cpp
