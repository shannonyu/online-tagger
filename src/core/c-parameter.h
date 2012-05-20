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
#ifndef __C_PARAMETER_H__
#define __C_PARAMETER_H__

#include "ltp-parameter.h"

using namespace std;
using namespace ltp::framework;

class CParameter : public Parameter {
public:
    CParameter(int handle, int size = 0);
    ~CParameter();

    void copy(CParameter *param);

    void averaged(double avValue);

    void update(int key, double value);

    void update(int key, double value, double curUpdSeq);

    double value(int key);

    int load(FILE *fp);

    int save(FILE *fp);

    int size();

// private:
    int m_Handle;
    int m_Size;
    double *m_Parameters;
    double *m_Total;
    double *m_Variance;
    double *m_Upd;
};

#endif  // end for __C_PARAMETER_H__
