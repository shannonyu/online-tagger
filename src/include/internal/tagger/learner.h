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

#ifndef __LEARNER_H__
#define __LEARNER_H__

#include "ltp-data.h"
#include "ltp-parameter.h"

class Learner {
public:
    Learner() {}
    ~Learner() {}

    void learn(Data *data, Parameter *param);
    Parameter *learnIter(Data *data, Parameter *param, int iter);
private:
};

#endif  // end for __LEARNER_H__
