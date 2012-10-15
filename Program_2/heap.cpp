// Robert Gruener
// ECE164 Data Structures and Algorithms II
// heap.cpp

#include "heap.h"

heap::heap(int capacity){
    this->capacity = capacity;
    this->filled = 0;
    data.resize(capacity+1);
    mapping = new hashTable(capacity*2);
}

int heap::insert(const std::string &id, int key, void *pv /*= NULL*/){
    if (this->filled >= this->capacity){
        return 1; // Heap already filled to capacity
    } else if (this->mapping->contains(id) == 1){
        return 2; // Node with given id already in heap
    }
    this->filled++;
    this->data[filled].id = id;
    this->data[filled].key = key;
    this->data[filled].pData = pv;
    this->mapping->insert(data[filled].id, &data[filled]);
    percolateUp(filled);
    return 0;
}

int heap::setKey(const std::string &id, int key){
    bool b;
    node *pn = static_cast<node *> (this->mapping->getPointer(id, &b));
    if (b){
        pn->key = key;
        int pos = getPos(pn);
        percolateUp(pos);
        percolateDown(pos);
        return 0;
    } else {
        return 1;
    }
}

int heap::deleteMin(std::string *pId /*= NULL*/, int *pKey /*= NULL*/, void *ppData /*= NULL*/){
    if (this->filled == 0){
        return 1;
    }
    if (pId){
        *pId = this->data[1].id;
    }
    if (pKey){
        *pKey = this->data[1].key;
    }
    if (ppData){
        *(static_cast<void **> (ppData)) = this->data[1].pData;
    }
    this->mapping->remove(this->data[1].id);
    this->data[1] = this->data[this->filled--]; // place last element at top of heap
    this->mapping->setPointer(this->data[1].id, &this->data[1]); // change pointer in hash table
    percolateDown(1);
    return 0;
}

int heap::remove(const std::string &id, int *pKey /*= NULL*/, void *ppData /*= NULL*/){
    bool b;
    node *pn = static_cast<node *> (this->mapping->getPointer(id, &b));
    if (b){
        if (pKey){
           *pKey = pn->key;
        }
        if (ppData){
            *(static_cast<void **> (ppData)) = pn->pData;
        }
        int pos = getPos(pn);
        this->data[pos].key = this->data[1].key - 1;
        percolateUp(pos);
        deleteMin();
        return 0;
    } else {
        return 1;
    }
    return 0;
}

void heap::percolateUp(int posCur){
    node tmp = this->data[posCur];
    while (posCur > 1 && tmp.key < this->data[posCur/2].key){
        this->data[posCur] = this->data[posCur/2];
        this->mapping->setPointer(this->data[posCur].id, &this->data[posCur]); // switch pointer in hash table
        posCur /= 2;
    }
    this->data[posCur] = tmp;
    this->mapping->setPointer(data[posCur].id, &data[posCur]);
}

void heap::percolateDown(int posCur){
    int left, right, min;
    node tmp = this->data[posCur];
    while (true){
        left = posCur*2; // left child index
        right = posCur*2 + 1; // right child index
        min = posCur; // index of minimum node between parents and children
        if (left <= this->filled && this->data[left].key < tmp.key){
            min = left;
        }
        if (right <= this->filled &&
                this->data[right].key < tmp.key && 
                this->data[right].key < this->data[left].key){
            min = right;
        }
        if (min != posCur){ // if one of children is less than parent, need to switch
            this->data[posCur] = this->data[min];
            this->mapping->setPointer(this->data[posCur].id, &this->data[posCur]); // switch pointer in hash table
            posCur = min;
        } else {
            break;
        }
    }
    this->data[posCur] = tmp;
    this->mapping->setPointer(this->data[posCur].id, &this->data[posCur]);
}

int heap::getPos(node *pn){
    return pn - &this->data[0];
}
