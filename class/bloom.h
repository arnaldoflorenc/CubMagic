#ifndef BLOOM_H
#define BLOOM_H

#include "Solver.h"
#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <stack>
#include <memory>
#include <queue>
#include <unordered_set>
#include <bitset>

using namespace std;

class bloom {
private:
    size_t filter_size;
    std::vector<bool> bits;
    int num_hashes;
    std::vector<size_t> salts;

    size_t hash_with_salt(size_t h, size_t salt) const {
        return std::hash<size_t>{}(h ^ salt) % filter_size;
    }

public:
    bloom(size_t filter_size = 1000000, int num_hashes = 4);

    void insert(size_t hash);

    bool mightContain(size_t hash);

    void clear();
};
#endif