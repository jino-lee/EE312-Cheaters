// EE 312 Lab 8 Milestone I
// Jin Lee (jl67888) and Andy Wu (amw5468)

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "hashTable.h"

using namespace std;

struct Output {
   int count;
   string file1;
   string file2;
}; typedef struct Output Output;

int getdir(string dir, vector<string> &files);
void clean(string &word);
bool outputCompare(Output i, Output j) { return (i.count > j.count); }


int main(int argc, char *argv[]) {
   // TRYING LARGER TABLE SIZE FOR BIG DOC SET - failed (program exits during tabulation)
   int tableSize = 100007;  
   //int tableSize = 500009;
   
   // uncomment to test different sized doc set inputs
   string dir = string(argv[1]);
   //string dir = string("med_doc_set");
   //string dir = string("big_doc_set");

   vector<string> files = vector<string>();
   int n = atoi(argv[2]); // n-word sequences   
   getdir(dir, files);
   HashTable *Hash = new HashTable; 

   //ifstream myfile; 
   // remove the first two elements in the files vector (.home directory and parent directory) before iterating
   files.erase(files.begin());
   files.erase(files.begin());
   for (int i = 0; i < files.size(); i++) {
      string fileptr = dir + "/" + files[i];
      // cout << endl << fileptr << endl; // DEBUG
      ifstream myfile;
      myfile.open(fileptr.c_str());
      
      if (myfile.is_open()) {
         string word;
	      vector<string> sequence; 
      
         while (!myfile.eof()) {// extract words from file
            while (sequence.size() < n && !myfile.eof()) {
               myfile >> word;
               clean(word);

               if (word.length() > 0) {
                  sequence.push_back(word);
               }
            }

            string nChunk;
            for (int j = 0; j < sequence.size(); j++) {
               nChunk = nChunk + sequence[j];
            }
            //cout << "currently before the HashTable's insert function is called..." << endl;

            Hash -> insert(nChunk, i); // i is the index of the current file of the files vector. Note the i = 0 and i = 1 are not used (root and parent files)
            
            
            //cout << nChunk << "inserted into Hash Table." << endl;
            

            sequence.erase(sequence.begin());
         } 
      }

      else cout << "Unable to open file" << endl;

      myfile.close();
   // cout << "DEBUG : iteration " << i << endl;
   } //at this point, done hashing every file
   
   // Dynamically allocate count 2D array
   int numRows = files.size();
   int numCols = files.size();
   // cout << "Currently right before dynamic allocation of 2D array..." << endl; // debugging cout
   int **countArray = new int*[numRows];
   for (int i = 0; i < numRows; i++) {
      countArray[i] = new int[numCols];
   }

   // initialize entries of the 2D countArray to 0
   for (int i = 0; i < numRows; i++) {
      for (int j = 0; j < numCols; j++) {
         countArray[i][j] = 0; 
      }
   }


   // for each file, iterate though the whole Hash Table to find hits
   // cout << "Currently right before tabulating all hits..." << endl; // debugging cout
   for (int i = 0; i < files.size(); i++) {
      for (int j = 0; j < tableSize; j++) {
         if (Hash -> getHead(j) != NULL) {
            Entry *temp = Hash -> getHead(j);
            bool foundFlag = false;
            //Entry *targetPtr = NULL;
            while (temp != NULL) { // iterate through linked list for target file #
               if (temp -> fileIdx == i) { 
                  foundFlag = true; 
                  break;
               }
               temp = temp -> next; // increment pointer
            }
            if (foundFlag) {
               temp = Hash -> getHead(j); // reset temp to point to start of linked list
               while (temp != NULL) {
                  if (temp -> fileIdx != i) { // don't count hits against the file itself
                     countArray[i][temp->fileIdx]++;
                  }
                  temp = temp -> next;
               }
               foundFlag = false; // reset flag before next iteration (for the next Hash Table slot)
            }
         }
      }
   }
   // cout << "Currently right after tabulating all hits..." << endl; // debugging cout

   // Print out the files names with the most hits above the given threshold here:
   // NOTE: STILL NEED TO SORT OUTPUT FROM MOST HITS TO LEAST AND GET THRESHOLD VALUE/"n" FOR N-WORD CHUNKS FROM COMMAND LINE
   // Only need to check upper triangle since plagiarism is a symmetric property
   int testThreshold = atoi(argv[3]);
   vector<Output> OutputSort;
   bool plagiarismFlag = false;
   for (int i = 0; i < numRows; i++) {
      for (int j = 0; j < numCols; j++) {
         if (i > j) { // only print from upper triangle  
            if (countArray[i][j] > testThreshold) {
               plagiarismFlag = true;
               // cout << countArray[i][j] << ": " << files[i] << " and " << files[j] << endl;
               Output temp;
               temp.count = countArray[i][j];
               temp.file1 = files[i];
               temp.file2 = files[j];
               OutputSort.push_back(temp);
            }
         }
      }
   }
   if (plagiarismFlag = false) {
      cout << endl << "None of the files exceeded the given threshold." << endl;
   }
   cout << endl;

   // print sorted list of outputs
   std::sort(OutputSort.begin(), OutputSort.end(), outputCompare);   
   cout << endl << "PLAGIARISM CHECKER RESULTS: " << endl;
   //for (vector<Output>::iterator i = OutputSort.begin(); i != OutputSort.end(); i++) {
   for (int i = 0; i < OutputSort.size(); i++) {
      cout << OutputSort[i].count << ": " << OutputSort[i].file1 << OutputSort[i].file2 << endl;
   }

   //debugging function to check contents of the HashTable (uncomment if you want to see HashTable)
   //Hash->printTable();
   
   // de-allocate 2D array
   for (int i = 0; i < numRows; i++) {
         delete[] countArray[i];
   }
   delete[] countArray;


   // destruct HashTable
   delete(Hash);

   return 0;
}



/* ---getdir---
   PRE: Takes string of directory name and address of string vector
   POST: Stores string of file names in directory in vector
*/
int getdir(string dir, vector<string> &files) {
   DIR *dp;
   struct dirent *dirp;
   if ((dp = opendir(dir.c_str())) == NULL) {
      cout << "Error(" << errno << ") opening " << dir << endl;
      return errno;
   }

   while ((dirp = readdir(dp)) != NULL) {
      files.push_back(string(dirp->d_name));
   }
   closedir(dp);
   return 0;
}

/* ---clean---
   PRE: Takes address of string to be cleaned
   POST: Removes punctuation and special characters, makes all char lowercase
*/
void clean(string &word) {
   for (int i = 0; i < word.length(); i++) {// check if each char is a lowercase alphabet
      if ('A' <= word[i] && word[i] <= 'Z') {// make lowercase
         word[i] = word[i] + 32;
      }
      else if (word[i] < 'a' || word[i] > 'z') {// remove special characters
         word.erase(word.begin() + i);
         i--;
      }
   }
}
