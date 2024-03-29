// Robert Gruener
// ECE165 DSA II
// graph.cpp

#include <iostream>
#include <fstream>
#include <limits>
#include "graph.h"
#include "heap.h"

#define INFINITY std::numeric_limits<int>::max()

graph::graph(int capacity /*= 100*/){
    vertices = new hashTable(capacity*2);
}

graph::graphVertex * graph::insertVertex(std::string vertex_id){
    if (this->vertices->contains(vertex_id)){
        return static_cast<graphVertex *> (this->vertices->getPointer(vertex_id));
    }
    graphVertex *new_vertex = new graphVertex();
    new_vertex->vertex_id = vertex_id;
    if (this->vertices->insert(vertex_id, new_vertex) != 0){
        std::cerr << "Insert to hash table failed on vertex: " << vertex_id << std::endl;
    }
    this->vertex_list.push_back(new_vertex);
    return new_vertex;
}

bool graph::containsVertex(std::string vertex_id){
    return this->vertices->contains(vertex_id);
}

bool graph::insertEdge(std::string src_vertex, std::string dest_vertex, int cost){
    bool b1, b2;
    graphVertex *src, *dest;
    
    src = this->insertVertex(src_vertex);
    dest = this->insertVertex(dest_vertex);

    if (src == NULL || dest == NULL){
        return false;
    }
    graphEdge new_edge;
    new_edge.cost = cost;
    new_edge.destination = dest;
    src->adjacency_list.push_back(new_edge);
    return true;
}

int graph::dijkstra(std::string source_vertex){
    heap heap_vertices(this->vertex_list.size()+1);
    graphVertex *v;
    int new_dist;

    std::list<graphVertex *>::iterator it;
    for (it=this->vertex_list.begin(); it!=vertex_list.end(); it++){
        if ((*it)->vertex_id == source_vertex){
            (*it)->distance = 0;
        } else {
            (*it)->distance = INFINITY;
        }
        (*it)->previous = NULL;
        heap_vertices.insert((*it)->vertex_id, (*it)->distance, (*it));
        (*it)->known=false;
    }

    while (heap_vertices.deleteMin(NULL, NULL, &v) != 1){
        v->known = true;
        std::list<graphEdge>::iterator it;
        if (v->distance == INFINITY){
            continue; // Continue if vertex cant be reached from source
        }
        for (it=v->adjacency_list.begin(); it!=v->adjacency_list.end(); it++){
            new_dist = v->distance + (*it).cost;
            if (new_dist < ((*it).destination)->distance){
                ((*it).destination)->distance = new_dist;
                heap_vertices.setKey(((*it).destination)->vertex_id, new_dist);
                ((*it).destination)->previous = v;
            }
        }
    }
}

void graph::printDijkstra(std::ofstream& outfile){
    std::list<graphVertex *>::iterator it;
    for (it=this->vertex_list.begin(); it!=vertex_list.end(); it++){
        outfile << (*it)->vertex_id << ": ";
        if ((*it)->distance == INFINITY){
            outfile << "NO PATH" << std::endl;
        } else {
            outfile << (*it)->distance << " [";
            this->printPath(outfile, *it);
            outfile << "]" << std::endl;
        }
    }
}

void graph::printPath(std::ofstream& outfile, graphVertex *v){
    if (v->previous != NULL){
        this->printPath(outfile, v->previous);
        outfile << ", ";
    }
    outfile  << v->vertex_id;

}
