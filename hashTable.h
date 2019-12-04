/* hashTable.h
 * EE 312
 * Jin Lee (jl67888) and Andy Wu (amw5468)
 * Implements a hash table class with separate chaining
*/
#ifndef _HASHTABLE_
#define _HASHTABLE

#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct Entry {
   int fileIdx;
   Entry *next;
}; 
typedef struct Entry Entry;

class HashTable {
   private:
      static const int tableSize = 100007; // number of entries
      Entry *head[tableSize];
   public:
      HashTable(); // constructor
      unsigned int hash(const string &key);
      void insert(const string &key, const int fileIdx);
      void printTable() const;
      ~HashTable();
};

#endif
