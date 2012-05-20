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

#include "seg-evaluator.h"

#include <iostream>
#include <string>
#include "cppstrlib.h"
using namespace ltp::utility;

SegmentEvaluator :: SegmentEvaluator(const char *file,
        Alphabet *charDict,
        Alphabet *labelDict) {

    FILE *fp = fopen(file, "r");
    char buff[2048];
    while (fgets(buff, 2048, fp)) {
        string line = cppstr::chomp(string(buff));
        vector<string> sep = cppstr::split(line);
        if (2 == sep.size() && "word" == sep[0]) {
            m_GoldWords.insert(sep[1]);
        }
    }
    fclose(fp);

    this->m_CharDict = charDict;
    this->m_LabelDict = labelDict;
}

SegmentEvaluator :: ~SegmentEvaluator() {
}

void
SegmentEvaluator :: start() {
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
    m_NumLabels = 0;
    m_NumCorrLabel = 0;
}

void
SegmentEvaluator :: end() {
}

void
SegmentEvaluator :: report() {
    fprintf(stderr, "Evaluating on %d instances ...\n"
            "* Total Number of labels %d\n"
            "* Total:      P=%.8lf(%d/%d)\tR=%.8lf(%d/%d)\tF=%.8lf\n"
            "* InLexicon:  P=%.8lf(%d/%d)\tR=%.8lf(%d/%d)\tF=%.8lf\n"
            "* OutLexicon: P=%.8lf(%d/%d)\tR=%.8lf(%d/%d)\tF=%.8lf\n"
            "* Labels:     p=%.8lf(%d/%d)\n",
            m_NumInstances,
            m_NumGoldWords,
            this->p(), m_NumCorrPredWords, m_NumPredWords,
            this->r(), m_NumCorrPredWords, m_NumGoldWords,
            this->f(),
            (double)m_NumCorrPredWordsInLexicon / m_NumPredWordsInLexicon,
            m_NumCorrPredWordsInLexicon, m_NumPredWordsInLexicon,
            (double)m_NumCorrPredWordsInLexicon / m_NumGoldWordsInLexicon,
            m_NumCorrPredWordsInLexicon, m_NumGoldWordsInLexicon,
            (double)m_NumCorrPredWordsInLexicon * 2 / (m_NumGoldWordsInLexicon + m_NumPredWordsInLexicon),
            (double)m_NumCorrPredWordsInLexicon / m_NumPredWordsInLexicon,
            m_NumCorrPredWordsOutLexicon, m_NumPredWordsOutLexicon,
            (double)m_NumCorrPredWordsOutLexicon / m_NumGoldWordsOutLexicon,
            m_NumCorrPredWordsOutLexicon, m_NumGoldWordsOutLexicon,
            (double)m_NumCorrPredWordsOutLexicon * 2 / (m_NumGoldWordsOutLexicon + m_NumPredWordsOutLexicon),
            (double)m_NumCorrLabel / m_NumLabels,
            m_NumCorrLabel, m_NumLabels);
}

void
SegmentEvaluator :: evaluate(Labels *result, Instance *inst, bool accumulate) {
    Items *items = inst->items();
    Labels *gold = inst->labels();

    if (result->size() != gold->size()) {
        return;
    }

    if (accumulate) {
        ++ m_NumInstances;
        vector<string> goldWords = this->buildWordsFromLabels(items, gold);
        vector<string> predWords = this->buildWordsFromLabels(items, result);


        for (int i = 0; i < result->size(); ++ i) {
            if (result->at(i) == gold->at(i)) {
                ++ m_NumCorrLabel;
            }
        }

        m_NumGoldWords += goldWords.size();
        m_NumPredWords += predWords.size();

        int len = 0;
        int goldLen = 0;
        int predLen = 0;
        for (int i = 0; i < goldWords.size(); ++ i)
            len += goldWords[i].size();

        for (int i = 0, j = 0;
                i < goldWords.size() && j < predWords.size(); ) {

            if (goldWords[i] == predWords[j]) {
                ++ m_NumCorrPredWords;
                bool inLexicon = (m_GoldWords.find(goldWords[i]) != m_GoldWords.end());
                if (inLexicon) {
                    ++ m_NumGoldWordsInLexicon;
                    ++ m_NumPredWordsInLexicon;
                    ++ m_NumCorrPredWordsInLexicon;
                } else {
                    ++ m_NumGoldWordsOutLexicon;
                    ++ m_NumPredWordsOutLexicon;
                    ++ m_NumCorrPredWordsOutLexicon;
                }
                goldLen += goldWords[i].size();
                predLen += predWords[j].size();

                ++ i;
                ++ j;
            } else {
                goldLen += goldWords[i].size();
                predLen += predWords[j].size();

                bool inLexicon;
                inLexicon = (m_GoldWords.find(goldWords[i]) != m_GoldWords.end());
                if (inLexicon) {
                    ++ m_NumGoldWordsInLexicon;
                } else {
                    ++ m_NumGoldWordsOutLexicon;
                }

                inLexicon = (m_GoldWords.find(predWords[j]) != m_GoldWords.end());
                if (inLexicon) {
                    ++ m_NumPredWordsInLexicon;
                } else {
                    ++ m_NumPredWordsOutLexicon;
                }

                ++ i;
                ++ j;


                while (goldLen < len && predLen < len) {
                    if (goldLen < predLen) {
                        goldLen += goldWords[i].size();
                        bool inLexicon = (m_GoldWords.find(goldWords[i]) != m_GoldWords.end());
                        if (inLexicon) {
                            ++ m_NumGoldWordsInLexicon;
                        } else {
                            ++ m_NumGoldWordsOutLexicon;
                        }
                        ++ i;
                    } else if (goldLen > predLen) {
                        predLen += predWords[j].size();
                        bool inLexicon = (m_GoldWords.find(predWords[j]) != m_GoldWords.end());
                        if (inLexicon) {
                            ++ m_NumPredWordsInLexicon;
                        } else {
                            ++ m_NumPredWordsOutLexicon;
                        }
                        ++ j;
                    } else {
                        break;
                    }
                }
            }
        }

        m_NumLabels += result->size();
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
        int numLabels = 0;
        int numCorrLabel = 0;

        numLabels += result->size();
    }
}

vector<string>
SegmentEvaluator :: buildWordsFromLabels(Items *items, Labels *labels) {
    vector<string> ret;
    string tag;
    string word;

    for (int i = 0; i < items->size(); ) {
        tag = m_LabelDict->rlookup(labels->at(i));
        if ("S" == tag) {
            word = "X";//m_CharDict->rlookup(items->at(i)->word());
            ret.push_back(word);
            ++ i;
        } else if ("B" == tag) {
            word = "";
            while ("E" != tag && i < items->size()) {
                word = word + "X";//m_CharDict->rlookup(items->at(i)->word());
                tag = m_LabelDict->rlookup(labels->at(i));
                ++ i;
            }
            ret.push_back(word);
        } else {
            cerr << "Exception catchecd." << endl;
            return vector<string>(0);
        }
    }

    return ret;
}

double
SegmentEvaluator :: p() {
    return (double)m_NumCorrPredWords/m_NumPredWords;
}

double
SegmentEvaluator :: r() {
    return (double)m_NumCorrPredWords/m_NumGoldWords;
}

double
SegmentEvaluator :: f() {
    return (double)m_NumCorrPredWords * 2.0 / (m_NumGoldWords + m_NumPredWords);
}
