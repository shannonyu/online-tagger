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
#ifndef __TAGGER_POSTAG_EVALUATOR_H__
#define __TAGGER_POSTAG_EVALUATOR_H__

#include "ltp-evaluator.h"
#include "ltp-alphabet.h"

using namespace ltp::framework;

#include "hash_map.hpp"

class PostagEvaluator : public Evaluator {
public:
    PostagEvaluator(const char *file,
            Alphabet *wordDict);
    ~PostagEvaluator();

    void start();
    void end();
    void report();
    void evaluate(Labels* result, Instance* inst, bool accumulate = false);
    double p();
    double r();
    double f();
private:
    hash_set<int> m_GoldWords;

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
};


#endif  // end for __TAGGER_POSTAG_EVALUATOR_H__
