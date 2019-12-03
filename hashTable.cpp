
#include <vector>
#include <string>
#include <iostream>
#include "hashTable.h"

using namespace std;

int hash(const string &key, int tableSize) {
   int idx = 0;
   int power = 1; 
   int prime =  11;
   for (int i = 0; i < key.length(); i++) {
      idx = idx + key[i] * power;
      power = power * prime;
   }   
   return idx % tableSize; 
}

