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
#include "seg-decoder.h"
#include "k-heap.h"
#include "cpp-data.h"
#include "cppstrlib.h"

#include <algorithm>

using namespace ltp::utility;

// ----------------------------------------------
// PLAIN DECODER
// ----------------------------------------------
SegmentDecoder::SegmentDecoder(
        Model *model,
        int agenda) {
    this->m_Model  = model;
    this->m_Agenda = agenda;
    Alphabet *labelAlpha = model->getAlphabet("LABELS");
    this->m_NumLabels = labelAlpha->size();

    m_Legal = new int *[m_NumLabels + 1];
    for (int i = 0; i <= m_NumLabels; ++ i) {
        char prev = 'X';
        if (i < m_NumLabels)
            prev = labelAlpha->rlookup(i)[0];

        m_Legal[i] = new int[m_NumLabels];
        for (int j = 0; j < m_NumLabels; ++ j) {
            char curr = labelAlpha->rlookup(j)[0];

            m_Legal[i][j] = 0;
            if ((prev == 'X' || prev == 'S' || prev == 'E') 
                    && (curr == 'S' || curr == 'B'))
                m_Legal[i][j] = 1;
            if ((prev == 'M' || prev == 'B') && 
                    (curr == 'M' || curr == 'E'))
                m_Legal[i][j] = 1;
        }
    }
}

SegmentDecoder::~SegmentDecoder() {
    for (int i = 0; i <= m_NumLabels; ++ i)
        delete [] m_Legal[i];
    delete [] m_Legal;
}

DecodeResults *
SegmentDecoder :: decode(Instance *inst, Parameter *param) {

    Items *items = inst->items();
    int len = items->size();
    int numFeatures = m_Model->getAlphabet("FEATURES")->size();
    int numLabels   = m_Model->getAlphabet("LABELS")->size();

    double **uniScoreCache = new double *[len];
    for (int i = 0; i < len; ++ i) {
        uniScoreCache[i] = new double[numLabels];
        Item *item = items->at(i);
        for (int label = 0; label < numLabels; ++ label) {
            uniScoreCache[i][label] = 0.0;

            int sz = item->size(label);
            for (int j = 0; j < sz; ++ j) {
                uniScoreCache[i][label] += param->value(item->at(j, label));
            }
        }
    }

    double **biScoreCache = new double *[numLabels + 1];
    for (int prevLabel = 0; prevLabel <= numLabels; ++ prevLabel) {
        biScoreCache[prevLabel] = new double[numLabels];
        for (int currLabel = 0; currLabel < numLabels; ++ currLabel) {
            biScoreCache[prevLabel][currLabel] = param->value(
                    numFeatures * numLabels
                    + prevLabel * numLabels
                    + currLabel);

        }
    }

    KHeap<DecodeState> **states = new KHeap<DecodeState> *[len];
    for (int i = 0; i < len; ++ i) {
        states[i] = new KHeap<DecodeState>[numLabels];
        for (int j = 0; j < numLabels; ++ j) {
            states[i][j].setK(m_Agenda);
        }
    }

    for (int i = 0; i < len; ++ i) {
        // fprintf(stderr, "i=%d\n", i);
        for (int currLabel = 0; currLabel < numLabels; ++ currLabel) {
            if (i == 0) {
                // fprintf(stderr, "!%d->%d\n", numLabels, currLabel);
                if (m_Legal[numLabels][currLabel] == 0) {
                    continue;
                }

                double score = uniScoreCache[0][currLabel] + biScoreCache[numLabels][currLabel];
                states[i][currLabel].insert(DecodeState(currLabel, score, NULL));
            } else {
                for (int prevLabel = 0; prevLabel < numLabels; ++ prevLabel) {
                    if (m_Legal[prevLabel][currLabel] == 0)
                        continue;
                    for (int j = 0; j < states[i - 1][prevLabel].size(); ++ j) {
                        DecodeState *prev = states[i - 1][prevLabel].at(j);
                        double score = prev->score + 
                            uniScoreCache[i][currLabel] + biScoreCache[prevLabel][currLabel];
                        // printf("#%d->%d\n", prevLabel, currLabel);
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


