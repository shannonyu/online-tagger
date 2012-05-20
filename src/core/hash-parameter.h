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
#ifndef __HASH_PARAMETER_H__
#define __HASH_PARAMETER_H__

#include "ltp-parameter.h"
#include <iostream>
#include "hash_map.hpp"

using namespace std;
using namespace ltp::framework;

class HashParameter : public Parameter {
public:
    HashParameter();
    ~HashParameter();

    void copy(HashParameter *param);
    void averaged(double avValue);
    void update(int key, double value);
    void update(int key, double value, double curUpdSeq);
    double value(int key);
    int load(FILE *fp);
    int save(FILE *fp);
    int size();
// private:
    hash_map<int, double> m_Parameters;
    hash_map<int, double> m_Total;
    hash_map<int, double> m_Variance;
    hash_map<int, double> m_Upd;
};

#endif  // end for __TAGGER_PARAMETER_H__
