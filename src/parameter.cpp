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
#include "parameter.h"
#include "string.h"

#include "cppstrlib.h"
#include "ltp-math-const.h"
#include <algorithm>

using namespace ltp::math;
using namespace ltp::utility;

CParameter :: CParameter(int handle, int size): m_Handle(handle), m_Size(size) {

    m_Parameters = NULL;
    m_Total      = NULL;
    m_Variance   = NULL;
    m_Upd        = NULL;

    if (handle & (1<<0)) {
        m_Parameters = new double[m_Size];
    }
    if (handle & (1<<1)) {
        m_Total = new double[m_Size];
    }
    if (handle & (1<<2)) {
        m_Variance = new double[m_Size];
    }
    if (handle & (1<<3)) {
        m_Upd = new double[m_Size];
    }

    for (int i = 0; i < m_Size; ++ i) {
        if (handle & (1<<0)) {
            m_Parameters[i] = 0.0;
        }
        if (handle & (1<<1)) {
            m_Total[i] = 0.0;
        }
        if (handle & (1<<2)) {
            m_Variance[i] = 0.0;
        }
        if (handle & (1<<3)) {
            m_Upd[i] = 0.0;
        }
    }
}

CParameter :: ~CParameter() {
    delete[] m_Parameters;
    delete[] m_Total;
    delete[] m_Variance;
    delete[] m_Upd;
}

void
CParameter :: averaged(double avValue) {
    for (int i = 0; i < m_Size; ++ i) {
        m_Total[i]      = m_Total[i] / avValue;
        m_Parameters[i] = m_Total[i];
    }
}

void
CParameter :: update(int key, double value) {
    if (key < 0 || key > m_Size) {
        return;
    }

    m_Parameters[key] += value;
}


inline void
CParameter :: update(int key, double value, double curUpdSeq) {
    if (key < 0 || key > m_Size) {
        return;
    }
    m_Total[key]      += (curUpdSeq - m_Upd[key]) * m_Parameters[key];
    m_Parameters[key] += value;
    m_Total[key]      += value;
    m_Upd[key]        =  curUpdSeq;
}

void
CParameter :: copy(CParameter *param) {
    if (m_Handle & (1<<0) && param->m_Handle & (1<<0)) {
        memcpy(m_Parameters, param->m_Parameters, param->size() * sizeof(double));
    }
    if (m_Handle & (1<<1) && param->m_Handle & (1<<1)) {
        memcpy(m_Total, param->m_Total, param->size() * sizeof(double));
    }
    if (m_Handle & (1<<2) && param->m_Handle & (1<<2)) {
        memcpy(m_Variance, param->m_Variance, param->size() * sizeof(double));
    }
    if (m_Handle & (1<<3) && param->m_Handle & (1<<3)) {
        memcpy(m_Upd, param->m_Upd, param->size() * sizeof(double));
    }
}

inline double
CParameter :: value(int key) {
    // return m_Parameters[key];
    if (key >= 0 && key < m_Size) {
        return m_Parameters[key];
    }
    return 0.0;
}

inline int
CParameter :: size() {
    return m_Size;
}

int
CParameter :: load(FILE *fp) {
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

    m_Size = cppstr::toInt(line);
    m_Parameters = new double[m_Size];
    for (int i = 0; i < m_Size; ++ i) {
        m_Parameters[i] = 0.0;
    }

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
CParameter :: save(FILE *fp) {
    fprintf(fp, "%d\n", m_Size);
    for (int i = 0; i < m_Size; ++ i) {
        if (m_Parameters[i] > ZERO || m_Parameters[i] < -ZERO) {
            fprintf(fp, "%d\t%.16lf\n", i, m_Parameters[i]);
        }
    }
    return 0;
}

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

