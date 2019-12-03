// EE 312 Lab 8 Milestone I
// Jin Lee (jl67888) and Andy Wu (amw5468)

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "hashTable.h"

using namespace std;

int getdir(string dir, vector<string> &files);
void clean(string &word);

int main() {
   string dir = string("sm_doc_set");
   vector<string> files = vector<string>();
   int n = 6; // n-word sequences   
   getdir(dir, files);
   
   //ifstream myfile; 
   for (int i = 2; i < files.size(); i++) { // starts at 2 to skip home directory & parent directory
      string fileptr = dir + "/" + files[i];
      cout << endl << fileptr << endl;
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
            cout << nChunk << endl;
    
            sequence.erase(sequence.begin());
         } 
      }

      else cout << "Unable to open file" << endl;

      myfile.close();
cout << "DEBUG : iteration " << i << endl;
   }
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
/* --- plagiarismCatcher---
   PRE: Takes command line parameters for the path from
   the executable program to the text files and n (length of word sequence)
   POST: Prints all n-word sequences to the console for a given n
*/ 

