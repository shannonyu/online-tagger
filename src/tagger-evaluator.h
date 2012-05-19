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

#ifndef __TAGGER_EVALUATOR_H__
#define __TAGGER_EVALUATOR_H__

#include "ltp-evaluator.h"
#include "ltp-data.h"

using namespace ltp::framework;

class TaggerEvaluator : public Evaluator {
public:
    TaggerEvaluator();
    ~TaggerEvaluator();

    void start();
    void end();
    void report();
    void evaluate(Labels *result, Instance *inst, bool accumulate = false);
    double p();
    double r();
    double f();

private:
    int m_NumInstances;
    int m_NumLabels;
    int m_NumCorrLabels;
};

#endif  // end for __TAGGER_EVALUATOR_H__
