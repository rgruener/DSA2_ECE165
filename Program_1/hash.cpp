// Robert Gruener
// ECE164 Data Structures and Algorithms II
// spellcheck.cpp
//

#include <vector>
#include <string>
#include <iostream>


class hashTable{

    public:
        // The constructor initializes the hash table.
        // Uses getPrime to choose a prime number at least as large as
        // the specified size for the initial size of the hash table.
        hashTable(int size = 0);

        // Insert the specified key into the hash table.
        // If an optional pointer is provided,
        // associate that pointer with the key.
        // Returns 0 on success,
        // 1 if key already exists in hash table,
        // 2 if rehash fails.
        int insert(const std::string &key, void *pv = NULL);

        // Check if the specified key is in the hash table.
        // If so, return true; otherwise, return false.
        bool contains(const std::string &key);

        // Get the pointer associated with the specified key.
        // If the key does not exist in the hash table, return NULL.
        // If an optional pointer to a bool is provided,
        // set the bool to true if the key is in the hash table,
        // and set the bool to false otherwise.
        void *getPointer(const std::string &key, bool *b = NULL);

        // Set the pointer associated with the specified key.
        // Returns 0 on success,
        // 1 if the key does not exist in the hash table.
        int setPointer(const std::string &key, void *pv);

        // Delete the item with the specified key.
        // Returns true on success,
        // false if the specified key is not in the hash table.
        bool remove(const std::string &key);

    private:

        // Each item in the hash table contains:
        // key - a string used as a key.
        // isOccupied - if false, this entry is empty,
        //              and the other fields are meaningless.
        // isDeleted - if true, this item has been lazily deleted.
        // pv - a pointer related to the key;
        //      NULL if no pointer was provided to insert.
        class hashItem {
            public:
                std::string key;
                bool isOccupied;
                bool isDeleted;
                void *pv;
        };

        int capacity; // The current capacity of the hash table.
        int filled;  // Number of occupied items in the table.

        std::vector<hashItem> data; // The actual entries are here.

        // The hash function.
        int hash(const std::string &key);

        // Search for an item with the specified key.
        // Return the position if found, -1 otherwise.
        int findPos(const std::string &key);

        // The rehash function; makes the hash table bigger.
        // Returns true on success, false if memory allocation fails.
        bool rehash();

        // Return a prime number at least as large as size.
        // Uses a precomputed sequence of selected prime numbers.
        static unsigned int getPrime(int size);
};

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
        if (data[index].key == key){
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
        if (data[index].key == key){
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
