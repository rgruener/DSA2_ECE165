// Robert Gruener
// ECE164 Data Structures and Algorithms II
// spellcheck.cpp

/* 
Programming Assignment #2: This program will take a file containing a
lit of commands as input, containing commands that create stacks and 
queues of certain data types and push or pop values to those lists. 
The output of the program will be placed in a file specified by the 
user.  
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <cmath>
#include <cstring>
#include <algorithm>

#include "hash.h"

using namespace std;

// Globals: File Streams, Dictionary, Valid Characters
ifstream infile, dictfile;
hashTable dictionary (50000);
ofstream outfile;
string valid_chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-\'";
string digits = "0123456789";


string toLowercase(string str){
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

void loadDictionary(){
    string line;
    int newline;
    while (getline(dictfile, line)){ // Read in line from dictionary file
        newline = line.find_last_not_of("\n");
        if (newline != string::npos){
            line.erase(newline+1);
        }
        if (line.length() > 20 || line.find_first_not_of(valid_chars) != string::npos){
            continue;
        }
        dictionary.insert(toLowercase(line));
    }
    dictfile.close();
}

void spell_check(){
    int newline, start_index, end_index, line_number = 1;
    string line, word;
    while (getline(infile, line)){
        newline = line.find_last_not_of("\n");
        if (newline != string::npos){
            line.erase(newline+1);
        }
        start_index = line.find_first_of(valid_chars);
        while (start_index != string::npos){
            end_index = line.find_first_not_of(valid_chars, start_index);
            word = toLowercase(line.substr(start_index, end_index-start_index));
            start_index = line.find_first_of(valid_chars, end_index);
            if (word.find_first_of(digits) != string::npos){
                continue;
            }
            else if (word.length() > 20){
                outfile << "Long word at line " << line_number << ", starts: " << word.substr(0, 20) << endl;
            }
            else if (!dictionary.contains(word)){
                outfile << "Unknown word at line " << line_number << ": " << word << endl;
            }
            if (end_index == string::npos){
                break;
            }
        }
        line_number++;
    }
}

int main(){

    string dictname,ifname,ofname,command;
    clock_t t1, t2;
    double timeDiff;
    cout << "Enter name of dictionary file: ";
    getline(cin, dictname);
    dictfile.open(dictname.c_str()); // open takes c string
    if (dictfile.is_open()){
        t1 = clock();
        loadDictionary();
        t2 = clock();
        timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;
        cout << "Total time (in seconds) to load dictionary: " << timeDiff << endl;
        dictfile.close();
    } else {
        cerr << "Error while opening dictionary file" << endl;
        return 0;
    }
    cout << "Enter name of input file: ";
    getline(cin, ifname);
    cout << "Enter name of output file: ";
    getline(cin, ofname);
    infile.open(ifname.c_str());
    outfile.open(ofname.c_str());
    if (infile.is_open() && outfile.is_open()){
        t1 = clock();
        spell_check();
        t2 = clock();
        timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;
        cout << "Total time (in seconds) to check document: " << timeDiff << endl;
        infile.close();
        outfile.close();
    } else {
        cerr << "Error while opening input/output file" << endl;
    }
	return 0;
}

