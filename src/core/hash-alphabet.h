/*
 *
 *
 *
 *
 *
 *
 *
 *
 */
#ifndef __HASH_DICT_H__
#define __HASH_DICT_H__

#include "ltp-alphabet.h"

#include <iostream>
#include <vector>
#include "hash_map.hpp"

using namespace std;
using namespace ltp::framework;

class HashDict : public Alphabet {
public:
    HashDict() : idx(0) {}
    ~HashDict() {}

    int lookup(const char *str);
    int insert(const char *str);

    int lookup(const string& str);
    int insert(const string& str);

    const char *rlookup(int key);

    int load(FILE *fp);
    int save(FILE *fp);

    int size();
private:
    int idx;
    vector<string> m_Redict;
    hash_map <string, int> m_Dict;
};

#endif  // end for __LTP_ALPHABET_H__
