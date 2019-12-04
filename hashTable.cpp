
#include <vector>
#include <string>
#include <iostream>
#include "hashTable.h"

using namespace std;

HashTable::HashTable() {
   for (int i = 0; i < tableSize; i++) {
      this -> head[i] = NULL; // initialize all pointers to NULL when Table is constructed
   }
}

// returns the mapped HashTable index for the given n-chunk input string
unsigned int HashTable::hash(const string &key) {
   unsigned int idx = 0;
   int power = 1; 
   int prime =  7;
   for (int i = 0; i < key.length(); i++) {
      idx = idx + (key[i] * power) ;
      //cout << "Value of idx is: " << idx << endl; // DETECTED INTEGER OVERFLOW FOR VARIABLE IDX CAUSING PROGRAM TO CRASH 
      power = power * prime;
   }   
   return idx % tableSize; 
}

void HashTable::insert(const string &key, const int fileIdx) {
         Entry *temp = new Entry;
         temp -> fileIdx = fileIdx;
         temp -> next = head[hash(key)];
         head[hash(key)] = temp;  
}

Entry* HashTable::getHead(int tableIdx) {
   return head[tableIdx];
}

// function to display hash table 
void HashTable::printTable() const { 
  for (int i = 0; i < tableSize; i++) { 
    cout << i;
    Entry *temp = head[i];
    while (temp != NULL) {
         cout << " --> " << temp -> fileIdx;
         temp = temp -> next; 
    }
    cout << endl; 
  } 
} 


HashTable::~HashTable() {
   for (int i = 0; i < tableSize; i++) {
      if (this -> head[i] != NULL) {
         Entry *headTemp = head[i];
         while (headTemp != NULL) {
            Entry *temp = headTemp;
            headTemp = headTemp -> next;
            delete(temp);
         }
      }
   }
}

