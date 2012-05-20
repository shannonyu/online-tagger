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
/*
 * Decoder with (word, postag) rule pruning.
 */
#include "pos-decoder.h"
#include "k-heap.h"
#include "cpp-data.h"
#include "cppstrlib.h"
#include <algorithm>

using namespace ltp::utility;

DecodeRule::DecodeRule(Alphabet *words,
        Alphabet *labels) : m_WordDict(words), m_LabelDict(labels) {
    int num_words  = m_WordDict->size();
    int num_labels = m_LabelDict->size();

    m_LegalMat = new bool[ num_words * num_labels ];
    for (int i = 0; i < num_words * num_labels; ++ i) {
        m_LegalMat[i] = true;
    }
}

DecodeRule::~DecodeRule() {
    delete[] m_LegalMat;
}

int DecodeRule::load(const char *filename) {

    int numWords  = m_WordDict->size();
    int numLabels = m_LabelDict->size();

    char buff[2048];
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        return -1;
    }

    string line;
    while (fgets(buff, 2048, fp) ) {
        line = cppstr::chomp( string(buff) );
        vector<string> sep = cppstr::split(line);

        if (line.size() <= 0) {
            continue;
        }

        if (sep[0] == "word") {
            int condition = m_WordDict->lookup( sep[1].c_str() );
            if (condition < 0) {
                continue;
            }

            for (int i = 0; i < numLabels; ++ i) {
                m_LegalMat[condition * numLabels + i] = false;
            }
            for (int i = 2; i < sep.size(); ++ i) {
                int tag = m_LabelDict->lookup( sep[i].c_str() );

                if (tag >= 0 && tag < numLabels) {
                    m_LegalMat[condition * numLabels + tag] = true;
                }
            }
        }
    }

    fclose(fp);

    return 0;
}

bool DecodeRule::legal(int condition, int tag) {
    if (condition >= 0 && condition < m_WordDict->size() &&
            tag >= 0 && tag < m_LabelDict->size()) {
        return m_LegalMat[condition * m_LabelDict->size() + tag];
    }
    return false;
}

DecodeResults* RuleDecoder::decode(Instance *inst, Parameter *param) {

    Items* items = inst->items();
    int len = items->size();
    int numFeatures = m_IdBuilder->numFeatures();
    int numLabels   = m_IdBuilder->numLabels();

    // Caching unigram feature score.
    double **uniScoreCache = new double*[len];
    for (int i = 0; i < len; ++ i) {
         uniScoreCache[i] = new double[numLabels];
    }

    for (int i = 0; i < len; ++ i) {
        Item *item = items->at(i);
        for (int label = 0; label < numLabels; ++ label) {

            uniScoreCache[i][label] = 0.0;
            if (m_Rule->legal(item->form(), label) == false) {
                continue;
            }

            int sz = item->size(label);
            for (int j = 0; j < sz; ++ j) {
                uniScoreCache[i][label] += param->value(item->at(j, label));
            }
        }
    }

    // Caching Bigram feature score.
    double **biScoreCache = new double*[numLabels + 1];
    // Pay attention to the Tag bigram("__BOT__", tag)
    for (int prevLabel = 0; prevLabel <= numLabels; ++ prevLabel) {
        biScoreCache[prevLabel] = new double[numLabels];

        for (int currLabel = 0; currLabel < numLabels; ++ currLabel) {
            biScoreCache[prevLabel][currLabel] = param->value(
                    m_IdBuilder->index(prevLabel, currLabel, true));
        }
    }

    KHeap<DecodeState> **states = new KHeap<DecodeState>*[len];
    for (int i = 0; i < len; ++ i) {
        states[i] = new KHeap<DecodeState>[numLabels];
        for (int j = 0; j < numLabels; ++ j) {
            states[i][j].setK(m_Agenda);
        }
    }

    // Accurate decode algorithm
    for (int i = 0; i < len; ++ i) {

        for (int currLabel = 0; currLabel < numLabels; ++ currLabel) {
            if ( m_Rule->legal( items->at(i)->form(), currLabel ) == false ) {
                continue;
            }
            if (i == 0 ) {
                double score = uniScoreCache[0][currLabel] + biScoreCache[numLabels][currLabel];
                states[i][currLabel].insert( DecodeState(currLabel, score, NULL) );
            } else {
                for (int prevLabel = 0; prevLabel < numLabels; ++ prevLabel) {
                    for (int j = 0; j < states[i - 1][prevLabel].size(); ++ j) {
                        DecodeState *prev = states[i - 1][prevLabel].at(j);
                        double score = prev->score + 
                            uniScoreCache[i][currLabel] + biScoreCache[prevLabel][currLabel];
                        states[i][currLabel].insert(DecodeState(currLabel, score, prev));
                    }
                }
            }
        }
    }

    vector<DecodeState> result_cache;

    for (int label = 0; label < numLabels; ++ label) {
        for (int i = 0; i < states[len - 1][label].size(); ++ i) {
            result_cache.push_back( *states[len - 1][label].at(i) );
        }
    }

    sort( result_cache.begin(), result_cache.end() );

    DecodeResults *ret = new CppDecodeResults();

    for (int i = 0; ((i < m_Agenda) && (result_cache.size() - i > 0)); ++ i) {
        Labels* single = new CppLabels(len);

        int pos = len - 1;
        DecodeState *now = &result_cache[result_cache.size() - i - 1];
        //DecodeState *now = result_cache.at(i);
        for (; pos >= 0; -- pos, now = now->prev) {
            single->set(now->label, pos);
        }
        ret->append( single );
    }

    for (int i = 0; i < len; ++ i)        { delete []uniScoreCache[i]; } delete []uniScoreCache;
    for (int i = 0; i <= numLabels; ++ i) { delete []biScoreCache[i];  } delete []biScoreCache;
    for (int i = 0; i < len; ++ i)        { delete []states[i];        } delete []states;

    return ret;
}
