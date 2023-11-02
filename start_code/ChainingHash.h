/*
 *  Separate chaining hashtable
 */

#ifndef __CHAINING_HASH_H
#define __CHAINING_HASH_H

// Standard library includes
#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>

// Custom project includes
#include "Hash.h"

// Namespaces to include
using std::vector;
using std::list;
using std::pair;
using std::iterator;
using std::cout;
using std::endl;

//
// Separate chaining based hash table - derived from Hash
//
template<typename K, typename V>
class ChainingHash : public Hash<K,V> {
private:
    int mSize; // Max Size
    int cSize;
    float load; // Load Factor
    vector<std::list <pair<K,V>>> HashVector; //Vector of lists
    bool rehash(){
        int tempsize = mSize;
        load = 0;        
        mSize = findNextPrime(mSize*2);
        HashVector.resize(mSize+1, list<pair<K,V>>());
        load = 0;
        for (int i = 0; i < tempsize; i++){
            load = 0;
            if (!HashVector[i].empty()){
                list<pair<K,V>> templist;
                for (typename list<pair<K,V>>::iterator j = HashVector[i].begin(); j != HashVector[i].end(); j++){
                    if (j->first%mSize == i){
                        templist.push_back(*j);
                    }
                    else {
                        HashVector[j->first%mSize].push_back(*j);
                    }
                }   
                templist.empty();
                std::swap(HashVector[i], templist);
            }
        }
        load = (float)cSize/(float)mSize;
        return true;
    }
public:
    ChainingHash(int n = 11) {
        HashVector.resize(n, list<pair<K,V>>());
        
        mSize = n;
        cSize, load = 0;
    }

    ~ChainingHash() {
        this->clear();
    }

    int size() {
        return cSize;
    }

    V operator[](const K& key) {
        int modkey = key%mSize;
        V rValue;
        if (!HashVector[modkey].empty()){
            
            typename list<pair<K,V>>::iterator current = HashVector[modkey].begin(); // finds first itterator of the HashVector
            bool found = false;
            for (int i = 0; i <= HashVector[modkey].size() && found == false; i++){
                if ((*current).first == key){                    
                    found = true;
                    rValue = current->first;
                }
                current++;
                
            }  
            if (found == true){
                return rValue;
            }
            
        }
        return -1;
    }

    bool insert(const std::pair<K, V>& pair) {
        HashVector[(pair.first)%mSize].push_back(pair);
        cSize++;
        load = (float)cSize/(float)mSize;
        if (load >= .75){
            rehash();
        }
        return true;
    }

    void erase(const K& key) {
        int modkey = key%mSize;
        bool check = false;
        for (typename list<pair<K,V>>::iterator i = HashVector[modkey].begin(); i != HashVector[modkey].end() && check != true; i++){
            if (i->first == key){
                check = true;
                HashVector[modkey].erase(i);
                cSize--;
                load = (float)cSize/(float)mSize;
            }
        }
    }

    void clear() {
        HashVector.clear();
        load, cSize = 0;
        mSize = HashVector.max_size();
    }

    int bucket_count() {
        return mSize;
    }

    float load_factor() {
        return load;
    }

private:
    int findNextPrime(int n)
    {
        while (!isPrime(n))
        {
            n++;
        }
        return n;
    }

    int isPrime(int n)
    {
        for (int i = 2; i <= sqrt(n); i++)
        {
            if (n % i == 0)
            {
                return false;
            }
        }

        return true;
    }

    int hash(const K& key) {
        return mSize%key;       
    }

};

#endif //__CHAINING_HASH_H
