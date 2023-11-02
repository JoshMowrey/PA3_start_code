#ifndef __PROBING_HASH_H
#define __PROBING_HASH_H

#include <vector>
#include <stdexcept>

#include "Hash.h"

using std::vector;
using std::pair;

// Can be used for tracking lazy deletion for each element in your table
enum EntryState {
    EMPTY = 0,
    VALID = 1,
    DELETED = 2
};

template<typename K, typename V>
class ProbingHash : public Hash<K,V> { // derived from Hash
private:
    // Needs a table and a size.
    // Table should be a vector of std::pairs for lazy deletion
    vector<pair<pair<K,V>,EntryState>> HashVector;
    int mSize, cSize;
    float load;
    bool rehash(){
        int oldsize = mSize;
        mSize = findNextPrime(mSize*2);
        HashVector.resize(mSize,pair<pair<K,V>,EntryState>());
        load = 0;
        vector<pair<pair<K,V>,EntryState>> tempvector(mSize, pair<pair<K,V>,EntryState>());
        for (int i = 0; i < oldsize; i++){
            if (HashVector[i] != pair<pair<K,V>,EntryState>()){
                if (tempvector[HashVector[i].first.first%mSize].first == std::pair<K,V>()){
                    tempvector[HashVector[i].first.first%mSize] = HashVector[i];
                }
        
                else{
                    for (int i = 0; i < oldsize; i++){
                        if (tempvector[((HashVector[i].first.first)+1)%mSize].first == std::pair<K,V>()){
                            tempvector[((HashVector[i].first.first)+1)%mSize] = HashVector[i];
                            i = oldsize;
                        }
                    }
                }
            }
        }
        std::swap(tempvector,HashVector);
        load = (float)cSize/(float)mSize;
        return true;
    }
public:
    ProbingHash(int n = 11) {
        HashVector.resize(n,pair<pair<K,V>,EntryState>());
        mSize = n;
        cSize, load = 0;
    }

    ~ProbingHash() {
        this->clear();
    }

    int size() {
        return cSize;
    }

    V operator[](const K& key) {
        if (HashVector[key%mSize].first.first == key){
            return HashVector[key%mSize].first.second;
        }   
        else{
            for (int i = 0; i < mSize; i++){
                if (HashVector[(key+1)%mSize].first.first == key){
                    return HashVector[(key+1)%mSize].first.second;
                }
            }
        }
    }

    bool insert(const std::pair<K, V>& pair) {
        if (load >= .75){
            rehash();
        }
        if (HashVector[pair.first%mSize].first == std::pair<K,V>() || HashVector[pair.first%mSize].second == DELETED){
            HashVector[pair.first%mSize].first = pair;
            HashVector[pair.first%mSize].second = VALID;
            cSize++;
            load = (float)cSize/(float)mSize;
            return true;
        }
        else{
            for (int i = 0; i < mSize; i++){
                if (HashVector[(pair.first+1)%mSize].first == std::pair<K,V>() || HashVector[(pair.first+1)%mSize].second == DELETED){
                    HashVector[(pair.first+1)%mSize].first = pair;
                    HashVector[(pair.first+1)%mSize].second = VALID;
                    cSize++;
                    load = (float)cSize/(float)mSize;
                    return true;
                }
            }
        }
        return false;
    }

    void erase(const K& key) {
        if (HashVector[key%mSize].first.second == key){
            HashVector[key].second = DELETED;

            cSize--;
            load = (float)cSize/(float)mSize;
            return;
        }   
        else{
            
            for (int i = 0; i < mSize; i++){
                if (HashVector[(key+1)%mSize].first.second == key){
                    HashVector[((key+1)%mSize)].second = DELETED;
                    cSize--;
                    load = (float)cSize/(float)mSize;
                    return;
                }
            }
        }
    }

    void clear() {
        HashVector.clear();
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
        return 0;       
    }
    
};

#endif //__PROBING_HASH_H
