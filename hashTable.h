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

typedef struct Entry {
   int idx;
   Entry *next;
} Entry; 

class HashTable {
   private:
      static const int tableSize = 100007; // number of entries
      Entry *T[tableSize];   
   public:
      int hash(const string &key, int tableSize);
      void insert(const string &key, int tableSize);
};

#endif
