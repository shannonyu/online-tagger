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

#include "tagger-evaluator.h"

#include <cstdio>

TaggerEvaluator :: TaggerEvaluator() {
}

TaggerEvaluator :: ~TaggerEvaluator() {
}

void
TaggerEvaluator :: start() {
    m_NumInstances  = 0;
    m_NumLabels     = 0;
    m_NumCorrLabels = 0;
}

void
TaggerEvaluator :: end() {
}

void
TaggerEvaluator :: report() {
}

void
TaggerEvaluator :: evaluate(Labels *result, Instance *inst, bool accumulate) {
    Items *items = inst->items();
    Labels *gold = inst->labels();

    if (result->size() != gold->size()) {
        fprintf(stderr, "ERROR: Label number not match.\n");
        return;
    }

    if(accumulate) {
        ++ m_NumInstances;
        for(int i = 0; i < result->size(); ++i) {
            ++ m_NumLabels;
            if(result->at(i) == gold->at(i)) {
                ++ m_NumCorrLabels;
            }
        }
    } else {
        int numLabels     = 0;
        int numCorrLabels = 0;
    }
}

double
TaggerEvaluator :: p() {
    return (double)m_NumCorrLabels / m_NumLabels;
}

double
TaggerEvaluator :: r() {
    return (double)m_NumCorrLabels / m_NumLabels;
}

double
TaggerEvaluator :: f() {
    return (double)m_NumCorrLabels / m_NumLabels;
}
