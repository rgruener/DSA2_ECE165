// Robert Gruener
// ECE165 DSA II
// graph.h

#ifndef _GRAPH_H
#define _GRAPH_H

#include <list>
#include <string>
#include <fstream>
#include "hash.h"

class graph {

    private:
        class graphEdge;

        class graphVertex {
            public:
                std::string vertex_id;
                bool known;
                int distance;
                graphVertex *previous;
                std::list<graphEdge> adjacency_list;
        };
        
        class graphEdge {
            public:
                int cost;
                graphVertex *destination;
        };

        std::list<graphVertex *> vertex_list;
        hashTable *vertices;

        // Helper function for printDijkstra
        void printPath(std::ofstream& outfile, graphVertex *v);

    public:
        // Constructor, capacity is only for initial size of 
        // hash table, which will increase as you insert more 
        // nodes.
        graph(int capacity=100);

        // Add new vertex to graph returns true
        // if successfully inserted vertex into 
        // graph, false otherwise
        // Useful for creating isolated vertices 
        // without any edges
        graphVertex* insertVertex(std::string vertex_id);

        // Returns true if vertex already in graph
        // returns false otherwise
        bool containsVertex(std::string vertex_id);

        // Checks if source and destination already in graph,
        // inserting them if not.  Then creates edge from source
        // to destination with cost
        bool insertEdge(std::string src_vertex, std::string dest_vertex, int cost);

        // Perform dijkstra's algorithm from a source
        // with vertex_id of start
        int dijkstra(std::string start);

        // Print results of Dijkstra's Algorithm to ofstream outfile
        void printDijkstra(std::ofstream& outfile);


};

#endif
