/*
 *
 *
 *
 *
 *
 *
 */
#ifndef __LTP_MATH_H__
#define __LTP_MATH_H__

#include "ltp-data.h"
#include "ltp-alphabet.h"
#include "ltp-parameter.h"
#include "ltp-math-const.h"

#include <iostream>
#include <vector>
#include <map>
#include <cmath>

using namespace std;
using namespace ltp::framework;

namespace ltp {

namespace math {


class SparseVector : public map<int, double> {
public:
    SparseVector() {}
    ~SparseVector() {}

    void add(int key, double value) {

        map<int, double>::iterator itx = this->find(key);

        if(itx == this->end()) {
            this->insert( make_pair(key, value) );
        } else {
            itx->second += value;
        }

        itx = this->find(key);
        if (itx->second > -EPS && itx->second < EPS) {
            this->erase(itx);
        }
    }

    double get(int key) {
        map<int, double>::iterator itx = this->find(key);
        if (itx != this->end()) {
            return itx->second;
        }
        return 0.0;
    }
};

double dotProduct(const SparseVector& x, const SparseVector& y) {
    double ret = 0.0;
    for (map<int, double>::const_iterator itx = x.begin(); itx != x.end(); ++ itx) {
        int key = itx->first;
        map<int, double>::const_iterator itx2 = y.find(key);
        if (itx2 != y.end()) {
            ret = ret + itx->second * itx2->second;
        }
    }
    return ret;
}

double score(const SparseVector& x, Parameter *param) {
    double ret = 0.0;

    for (map<int, double>::const_iterator itx = x.begin(); itx != x.end(); ++ itx) {
        ret += param->value(itx->first) * itx->second;
    }
    return ret;
}

void hildreth(const vector<SparseVector>& a, double* b, double* alpha, int K) {

    int max_iter = 10000;

    double* F   = new double[K]; for (int i = 0; i < K; ++ i) F[i]   = 0.0;
    double* kkt = new double[K]; for (int i = 0; i < K; ++ i) kkt[i] = 0.0;

    double max_kkt = NINF;

    double** A = new double*[K];
    for (int i = 0; i < K; ++ i) {
        A[i] = new double[K];
        for (int j = 0; j < K; ++ j) A[i][j] = 0.0;
    }

    bool* is_computed = new bool[K]; for (int i = 0; i < K; ++ i) is_computed[i] = false;

    int i;
    int max_kkt_i = -1;

    for(i = 0; i < K; i++) A[i][i] = dotProduct(a[i], a[i]);
    for(i = 0; i < K; i++) {
        F[i] = b[i];
        kkt[i] = b[i];
        if(kkt[i] > max_kkt) { max_kkt = kkt[i]; max_kkt_i = i; }
    }

    int iter = 0;
    double diff_alpha;
    double try_alpha;
    double add_alpha;

    while(max_kkt >= EPS && iter < max_iter) {

        diff_alpha = A[max_kkt_i][max_kkt_i] <= ZERO ? 0.0 : F[max_kkt_i] / A[max_kkt_i][max_kkt_i];
        try_alpha  = alpha[max_kkt_i] + diff_alpha;
        add_alpha  = 0.0;

        if(try_alpha < 0.0) {
            add_alpha = -1.0 * alpha[max_kkt_i];
        } else {
            add_alpha = diff_alpha;
        }

        alpha[max_kkt_i] = alpha[max_kkt_i] + add_alpha;

        if (!is_computed[max_kkt_i]) {
            for(i = 0; i < K; i++) {
                A[i][max_kkt_i] = dotProduct(a[max_kkt_i], a[i]);
                is_computed[max_kkt_i] = true;
            }
        }

        for(i = 0; i < K; i++) {

            F[i]  -= add_alpha * A[i][max_kkt_i];
            kkt[i] = F[i];
            if(alpha[i] > ZERO)
                kkt[i] = fabs(F[i]);
//                kkt[i] = abs(F[i]);
        }

        max_kkt   = NINF;
        max_kkt_i = -1;
        for(i = 0; i < K; i++) {
            if(kkt[i] > max_kkt) { max_kkt = kkt[i]; max_kkt_i = i; }
        }
        iter++;
    }

// FREE:
    for (int i = 0; i < K; ++ i) {
        delete[] A[i];
    }

    delete[] A;
    delete[] F;
    delete[] kkt;
    delete[] is_computed;
}

}

}

#endif  // end for __LTP_MATH_H__
