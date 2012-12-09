// Robert Gruener
// ECE165 Data Structures and Algorithms II
// prob_g.cpp

/* 
Programming Assignment #4: This program will take a file containing a
list of words as input, and determine whether every third word is a 
valid merge of the previous 2 words.  The output will go to a file
specified by the user
*/

#include <iostream>
#include <fstream>
#include <cstdlib>

#define MATRIX_SIZE 1001

using namespace std;

// Function for debugging
void print_matrix(string word_1, string word_2, string pos_merge, bool is_merge[MATRIX_SIZE][MATRIX_SIZE]){
    int i,j;
    cout << "MATRIX FOR " << pos_merge << ": " << endl;
    cout << "\t";
    for (i=0; i<word_1.length()+1; i++)
        cout << "\t" << word_1[i];
    cout << endl;
    for (i=0; i<word_2.length()+1; i++){
        for (j=0; j<word_1.length()+1; j++){
            if (j==0 and i>0)
                cout << word_2[i-1] << "\t";
            else if (j==0 and i==0)
                cout << "\t";
            cout << is_merge[i][j] << "\t";
        }
        cout << endl;
    }
}

int main(){

    ifstream infile;
    ofstream outfile;
    string ifname, ofname, word_1, word_2, pos_merge;
    bool is_merge[MATRIX_SIZE][MATRIX_SIZE];
    int i,j;
    cout << "Enter name of input file: ";
    getline(cin, ifname);
    cout << "Enter name of output file: ";
    getline(cin, ofname);
    infile.open(ifname.c_str());
    outfile.open(ofname.c_str());
    if (infile.is_open() && outfile.is_open()){
        while (getline(infile, word_1)){
            if (!(getline(infile, word_2) && getline(infile, pos_merge))){
                cerr << "Error: Invalid Input Must Specify 2 Words then Possible Merge" << endl;
                exit(1);
            }
            for (i=0; i<word_2.length()+1; i++){ // Initialize Matrix
                for (j=0; j<word_1.length()+1; j++){
                    if (i==0 and j>0){
                        if (pos_merge[j-1] == word_1[j-1]){
                            is_merge[i][j] = j==1? true : is_merge[i][j-1];
                            continue;
                        }
                    } else if (j==0 and i>0){
                        if (pos_merge[i-1] == word_2[i-1]){
                            is_merge[i][j] = i==1? true : is_merge[i-1][j];
                            continue;
                        }
                    }
                    is_merge[i][j] = false;
                }
            }
            for (i=1; i<word_2.length()+1; i++){
                for (j=1; j<word_1.length()+1; j++){
                    bool ans = false;
                    if (is_merge[i-1][j] or is_merge[i][j-1]){
                        if (word_2[i-1] == word_1[j-1] and pos_merge[i+j-1] == word_1[j-1]){
                            ans = true;
                        } else if (is_merge[i-1][j]){
                            if (pos_merge[i+j-1] == word_2[i-1]){
                                ans = true;
                            }
                        } else if (is_merge[i][j-1]){
                            if (pos_merge[i+j-1] == word_1[j-1]){
                                ans = true;
                            }
                        }
                    }
                    is_merge[i][j] = ans;
                }
            }
            print_matrix(word_1, word_2, pos_merge, is_merge);
            if (is_merge[word_2.length()][word_1.length()]){
                i = word_2.length();
                j = word_1.length();
                while (j>0 and i>= 0){
                    if (is_merge[i][j] and (i == 0 or !is_merge[i-1][j])){
                        pos_merge[i+j-1] = toupper(pos_merge[i+j-1]);
                        j--;
                    } else {
                        i--;
                    }
                }
                outfile << pos_merge << endl;
            } else {
                outfile << "*** NOT A MERGE ***" << endl;
            }
        }
        infile.close();
        outfile.close();
    } else {
        cerr << "Error while opening input/output file" << endl;
    }
	exit(1);
}

