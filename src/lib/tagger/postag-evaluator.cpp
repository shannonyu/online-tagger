/*
 *
 *
 *
 *
 *
 *
 *
 */

#include "postag-evaluator.h"
#include "cppstrlib.h"
using namespace ltp::utility;

#include <cstdio>

PostagEvaluator :: PostagEvaluator(const char *file,
        Alphabet *wordDict) {

    m_NumInstances = 0;
    m_NumGoldWords = 0;
    m_NumPredWords = 0;
    m_NumGoldWordsInLexicon = 0;
    m_NumPredWordsInLexicon = 0;
    m_NumGoldWordsOutLexicon = 0;
    m_NumPredWordsOutLexicon = 0;
    m_NumCorrPredWords = 0;
    m_NumCorrPredWordsInLexicon = 0;
    m_NumCorrPredWordsOutLexicon = 0;

    FILE *fp = fopen(file, "r");
    char buff[2048];

    while(fgets(buff, 2048, fp)) {
        string line = cppstr::chomp(string(buff));
        vector<string> sep = cppstr::split(line);
        if("word" == sep[0]) {
            int idx = wordDict->lookup(sep[1]);
            if(-1 != idx) {
                m_GoldWords.insert(idx);
            }
        }
    }

    fclose(fp);
}

PostagEvaluator :: ~PostagEvaluator() {
}

void
PostagEvaluator :: start() {
    m_NumInstances = 0;
    m_NumGoldWords = 0;
    m_NumPredWords = 0;
    m_NumGoldWordsInLexicon = 0;
    m_NumPredWordsInLexicon = 0;
    m_NumGoldWordsOutLexicon = 0;
    m_NumPredWordsOutLexicon = 0;
    m_NumCorrPredWords = 0;
    m_NumCorrPredWordsInLexicon = 0;
    m_NumCorrPredWordsOutLexicon = 0;
}

void
PostagEvaluator :: end() {
}

void
PostagEvaluator :: report() {
    fprintf(stderr, "Evaluating on %d instances ...\n"
            "* Total Number of labels %d\n"
            "* Total: P=%.8lf\n"
            "* InLexicon: P=%.8lf\n"
            "* OutLexicon: P=%.8lf\n",
            m_NumInstances,
            m_NumGoldWords,
            this->p(),
            (double)m_NumCorrPredWordsInLexicon / m_NumGoldWordsInLexicon,
            (double)m_NumCorrPredWordsOutLexicon / m_NumGoldWordsOutLexicon);

}

void
PostagEvaluator :: evaluate(Labels* result, Instance* inst, bool accumulate) {
    Items *items = inst->items();
    Labels *gold = inst->labels();

    if (result->size() != gold->size()) {
        cerr << "No match result and gold labels." << endl;
        return;
    }

    if (accumulate) {
        ++ m_NumInstances;
        for (int i = 0; i < result->size(); ++ i) {
            bool inLexicon = m_GoldWords.find(items->at(i)->form()) != m_GoldWords.end();
            if (inLexicon) {
                ++ m_NumGoldWordsInLexicon;
                ++ m_NumPredWordsInLexicon;
                if (result->at(i) == gold->at(i)) {
                    ++ m_NumCorrPredWords;
                    ++ m_NumCorrPredWordsInLexicon;
                }
            } else {
                ++ m_NumGoldWordsOutLexicon;
                ++ m_NumPredWordsOutLexicon;
                if ( result->at(i) == gold->at(i) ) {
                    ++ m_NumCorrPredWords;
                    ++ m_NumCorrPredWordsOutLexicon;
                }
            }
        }

        m_NumGoldWords += gold->size();
        m_NumPredWords += gold->size();
    } else {
        int numGoldWords = 0;
        int numPredWords = 0;
        int numGoldWordsInLexicon = 0;
        int numPredWordsInLexicon = 0;
        int numGoldWordsOutLexicon = 0;
        int numPredWordsOutLexicon = 0;
        int numCorrPredWords = 0;
        int numCorrPredWordsInLexicon = 0;
        int numCorrPredWordsOutLexicon = 0;

        for (int i = 0; i < result->size(); ++ i) {
            bool inLexicon = m_GoldWords.find(items->at(i)->form()) != m_GoldWords.end();
            if (inLexicon) {
                ++ numGoldWordsInLexicon;
                ++ numPredWordsInLexicon;
                if (result->at(i) == gold->at(i)) {
                    ++ numCorrPredWords;
                    ++ numCorrPredWordsInLexicon;
                }
            } else {
                ++ numGoldWordsOutLexicon;
                ++ numPredWordsOutLexicon;
                if ( result->at(i) == gold->at(i) ) {
                    ++ numCorrPredWords;
                    ++ numCorrPredWordsOutLexicon;
                }
            }
        }

        numGoldWords += gold->size();
        numPredWords += gold->size();

    }
}

double
PostagEvaluator :: p() {
    return (double)m_NumCorrPredWords / m_NumGoldWords;
}

double
PostagEvaluator :: r() {
    return this->p();
}

double
PostagEvaluator :: f() {
    return this->p();
}

