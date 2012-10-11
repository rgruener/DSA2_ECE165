// Robert Gruener
// ECE164 Data Structures and Algorithms II
// hash.cpp

#include <iostream>
#include "hash.h"

hashTable::hashTable(int size /*= 0*/){
    this->capacity = getPrime(size);
    data.resize(this->capacity);
    for (int i=0; i < this->capacity; i++){
        data[i].isOccupied = false; // Clear out new data vector
    }
}

int hashTable::insert(const std::string &key, void *pv /*= NULL*/){
    int index = hash(key);
    if (this->filled >= this->capacity / 2){
        if (!this->rehash()){
            return 2;
        }
    }
    while (data[index].isOccupied == true){
        if (data[index].key == key && !data[index].isDeleted){
            return 1;
        }
        if (index + 1 >= this->capacity){
            index = 0;
        } else {
        index++;
        }
    }
    data[index].isOccupied = true;
    data[index].key = key;
    if (pv){
        data[index].pv = pv;
    }
    this->filled++;
    return 0;
}

bool hashTable::contains(const std::string &key){
    if (findPos(key) == -1){
        return false;
    }
    return true;            
}

void * hashTable::getPointer(const std::string &key, bool *b /*= NULL*/){
    int index = this->findPos(key);
    if (index == -1){
        if (b){
            *b = false;
        }
        return NULL;
    } else {
        if (b){
            *b = true;
        }
        return this->data[index].pv;
    }
}

int hashTable::setPointer(const std::string &key, void *pv){
    int index = this->findPos(key);
    if (index == -1){
        return 1;
    } else {
        this->data[index].pv = pv;
    }
    return 0;
}

bool hashTable::remove(const std::string &key){
    int index = this->findPos(key);
    if (index == -1){
        return false;
    } else {
        this->data[index].isDeleted = true;
    }
    return true;
}

int hashTable::hash(const std::string &key){
    int hashVal = 0;

    for (int i=0;i<key.length();i++)
        hashVal = 37 * hashVal + key[i];
    hashVal %= this->capacity;
    if (hashVal < 0)
        hashVal += this->capacity;

    return hashVal;
}

int hashTable::findPos(const std::string &key){
    int index = hash(key);
    while (data[index].isOccupied){
        if (data[index].key == key && !data[index].isDeleted){
            return index;
        }
        if (index + 1 >= this->capacity){
            index = 0;
        } else {
            index++;
        }
    }
    return -1;
}

bool hashTable::rehash(){
    std::vector<hashItem> old_data = data;
    try{
        data.resize(this->capacity = getPrime(capacity * 2)); // Resize data vector while setting capacity
    }
    catch (std::bad_alloc& ba){
        std::cerr << "Memory Allocation Error: " << ba.what() << std::endl;
        return false;
    }
    for (int i=0; i < this->capacity; i++){
        data[i].isOccupied = false; // Clear out new data vector
    }

    for (int i=0; i < old_data.size(); i++){
        if (old_data[i].isOccupied){
            this->insert(old_data[i].key, old_data[i].pv);
        }
    }
    return true;
}

unsigned int hashTable::getPrime(int size){
    const int primes[] = {24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 1252829, 25165843};
    int i;
    for (i=0; i < sizeof(primes); i++){
        if (primes[i] >= size){
            break;
        }
    }
    return primes[i];
}
