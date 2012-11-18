// Robert Gruener
// ECE165 DSA 2
// dijkstra.cpp

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>

#include "heap.h"
#include "hash.h"
#include "graph.h"

using namespace std;

void loadGraph(ifstream& graphfile, graph &myGraph) {
    string line, vertex_1, vertex_2;
    int index1, index2, newline;
    int cost;

    //while (graphfile >> vertex_1 >> vertex_2 >> cost){
        ////cout << "Your edge is from " << vertex_1 << " to " << vertex_2 << " with a cost of " << cost << endl;
        //myGraph.insertEdge(vertex_1, vertex_2, cost);
    //}
    while (getline(graphfile, line)){
        newline = line.find_last_not_of("\n");
        if (newline != string::npos){
            line.erase(newline+1);
        }
        if (!line.length()){
            continue;
        }
        index1 = line.find(' ');
        index2 = line.rfind(' ');
        vertex_1 = line.substr(0, index1);
        vertex_2 = line.substr(index1+1, index2-index1-1);
        cost = atoi(line.substr(index2+1).c_str());
        myGraph.insertEdge(vertex_1, vertex_2, cost);
    }
}

int main() {
    string graph_fname, out_fname, start;
    ifstream graphfile;
    ofstream outfile;
    clock_t t1, t2;
    long double timeDiff;
    bool valid_start = false;
    graph myGraph(10000);
    cout << "Enter name of graph file: ";
    getline(cin, graph_fname);
    graphfile.open(graph_fname.c_str());
    if (graphfile.is_open()){
        loadGraph(graphfile, myGraph);
    } else {
        cerr << "Error while opening " << graph_fname << endl;
        exit (1);
    }
    while (!valid_start){
        cout << "Enter a valid vertex id for the starting vertex: ";
        getline(cin, start);
        valid_start = myGraph.containsVertex(start);
    }
    t1 = clock();
    myGraph.dijkstra(start);
    t2 = clock();
    timeDiff = ((long double) (t2 - t1)) / CLOCKS_PER_SEC;
    cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << timeDiff << endl;
    cout << "Enter name of output file: ";
    getline(cin, out_fname);
    outfile.open(out_fname.c_str());
    if (outfile.is_open()){
        myGraph.printDijkstra(outfile);
    } else {
        cerr << "Error while opening " << out_fname << endl;
    }

    exit (1);
}
