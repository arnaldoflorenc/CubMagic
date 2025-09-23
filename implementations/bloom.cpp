#include "../class/bloom.h"


bloom::bloom(size_t filter_size, int num_hashes) : filter_size(filter_size), bits(filter_size, false), num_hashes(num_hashes) {
    salts.resize(num_hashes);
    for (int i = 0; i < num_hashes; ++i) {
        salts[i] = 0x9e3779b9 + i * 0x7f4a7c15;
    }
}

void bloom::insert(size_t hash) {
    for (int i = 0; i < num_hashes; ++i) {
        size_t index = hash_with_salt(hash, salts[i]);
        bits[index] = true;
    }
}

bool bloom::mightContain(size_t hash) {
    for (int i = 0; i < num_hashes; ++i) {
        size_t index = hash_with_salt(hash, salts[i]);
        if (!bits[index]) {
            return false;
        }
    }
    return true;
}

void bloom::clear() {
    std::fill(bits.begin(), bits.end(), false);
}