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
#include "hash-alphabet.h"
#include "cppstrlib.h"

using namespace ltp::utility;

int
HashDict :: lookup(const char *str) {
    string buff = str;
    if (m_Dict.find( buff ) != m_Dict.end()) {
        return m_Dict[buff];
    }
    return -1;
}

int
HashDict :: lookup(const string& str) {
    return lookup(str.c_str());
}

int
HashDict :: insert(const char *str) {
    string buff = str;

    if (m_Dict.find( buff ) != m_Dict.end()) {
        return m_Dict[buff];
    } else {
        m_Redict.push_back(buff);
        m_Dict[buff] = idx ++;
        return idx - 1;
    }
}

int
HashDict :: insert(const string& str) {
    return insert(str.c_str());
}

const char *
HashDict :: rlookup(int rank) {
    if (rank >= 0 && rank < m_Redict.size()) {
        return m_Redict[rank].c_str();
    }
    return NULL;
}

int
HashDict :: load(FILE *fp) {
    if (NULL == fp) {
        return -1;
    }

    char buff[2048];
    idx = 0;

    while (fgets(buff, 2048, fp)) {
        string line = cppstr::chomp(buff);
        if ("#END#" == line) {
            break;
        }

        vector<string> sep = cppstr::split(line);
        if (2 != sep.size() ||
                false == cppstr::isInt(sep[1])) {
            fprintf(stderr, "ERROR: [%d] %s\n", idx, line.c_str());
            return -1;
        }

        ++ idx;
        m_Dict[sep[0]] = cppstr::toInt(sep[1]);
    }

    m_Redict.resize(idx);
    for (hash_map<string, int>::iterator itx = m_Dict.begin();
            itx != m_Dict.end(); ++ itx) {
        m_Redict[itx->second] = itx->first;
    }

    return 0;
}

int
HashDict :: save(FILE *fp) {
    for (hash_map<string, int>::iterator itx = m_Dict.begin();
            itx != m_Dict.end(); ++ itx) {
        fprintf(fp, "%s\t%d\n", itx->first.c_str(), itx->second);
    }
}

int
HashDict :: size() {
    return idx;
}
