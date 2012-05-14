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
#ifndef __TAGGER_SEGMENT_EVALUATOR_H__
#define __TAGGER_SEGMENT_EVALUATOR_H__

#include "ltp-evaluator.h"
#include "ltp-alphabet.h"
#include "hash_map.hpp"

using namespace ltp::framework;

#include <iostream>
#include <vector>
using namespace std;

class SegmentEvaluator : public Evaluator {
public:
    SegmentEvaluator(const char *file,
            Alphabet *charDict,
            Alphabet *labelDict);
    ~SegmentEvaluator();

    void start();
    void end();
    void report();
    void evaluate(Labels *reuslt, Instance *inst, bool accumulate = false);

    double p();
    double r();
    double f();
private:
    vector<string> buildWordsFromLabels(Items *items, Labels *labels);

private:
    hash_set<string> m_GoldWords;

    int m_NumInstances;
    int m_NumGoldWords;
    int m_NumPredWords;
    int m_NumGoldWordsInLexicon;
    int m_NumPredWordsInLexicon;
    int m_NumGoldWordsOutLexicon;
    int m_NumPredWordsOutLexicon;
    int m_NumCorrPredWords;
    int m_NumCorrPredWordsInLexicon;
    int m_NumCorrPredWordsOutLexicon;
    int m_NumLabels;
    int m_NumCorrLabel;

    Alphabet *m_CharDict;
    Alphabet *m_LabelDict;
};

#endif  // end for __TAGGER_SEGMENT_EVALUATOR_H__
