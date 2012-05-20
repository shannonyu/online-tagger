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
#include "hash-parameter.h"
#include "string.h"

#include "cppstrlib.h"
#include "ltp-math-const.h"
#include <algorithm>

using namespace ltp::math;
using namespace ltp::utility;

HashParameter :: HashParameter() {
}

HashParameter :: ~HashParameter() {
}

void
HashParameter :: copy(HashParameter *param) {
    m_Parameters = param->m_Parameters;
    m_Total      = param->m_Total;
    m_Upd        = param->m_Upd;
}

void
HashParameter :: averaged(double avValue) {
    for (hash_map<int, double>::iterator itx = m_Parameters.begin();
            itx != m_Parameters.end(); ++ itx) {
        m_Parameters[itx->first] = m_Total[itx->first] / avValue;
    }
}

inline void
HashParameter :: update(int key, double value) {
    m_Parameters[key] += value;
}

inline void
HashParameter :: update(int key, double value, double curUpdSeq) {
    m_Total[key]      += (curUpdSeq - m_Upd[key]) * m_Parameters[key];
    m_Parameters[key] += value;
    m_Total[key]      += value;
    m_Upd[key]        =  curUpdSeq;
}

inline double
HashParameter :: value(int key) {
    hash_map<int, double>::const_iterator itx = m_Parameters.find(key);

    if (itx != m_Parameters.end()) {
        return itx->second;
    } else {
        return 0.0;
    }
}

int
HashParameter :: size() {
    return m_Parameters.size();
}

int
HashParameter :: load(FILE *fp) {
    if (NULL == fp) {
        return -1;
    }

    char buff[2048];
    if (NULL == fgets(buff, 2048, fp)) {
        return -1;
    }
    string line = cppstr::chomp(string(buff));
    if (false == cppstr::isInt(line)) {
        return -1;
    }

    m_Parameters.clear();

    while (fgets(buff, 2048, fp)) {
        line = cppstr::chomp(string(buff));
        if ("#END#" == line) {
            break;
        }
        vector<string> sep = cppstr::split(line);
        if (false == cppstr::isInt(sep[0]) ||
                false == cppstr::isDouble(sep[1])) {
            return -1;
        }
        int key = cppstr::toInt(sep[0]);
        double value = cppstr::toDouble(sep[1]);
        m_Parameters[key] = value;
    }
    return 0;
}

int
HashParameter :: save(FILE *fp) {
    fprintf(fp, "%d\n", m_Parameters.size());
    for (hash_map<int, double> :: iterator itx = m_Parameters.begin();
            itx != m_Parameters.end(); ++ itx) {
        fprintf(fp, "%d\t%.16lf\n", itx->first, itx->second);
    }
    return 0;
}

