/*
 *
 *
 *
 *
 *
 *
 */

#include "mira-trainer.h"

#include "ltp-data.h"
#include "ltp-math.h"
using namespace ltp::math;

MiraTrainer::MiraTrainer(Evaluator *evaluator,
        Model *model) {
    this->m_Evaluator = evaluator;
    this->m_Model     = model;

    this->m_NumFeatures = model->getAlphabet("FEATURES")->size();
    this->m_NumLabels   = model->getAlphabet("LABELS")->size();
}

MiraTrainer::~MiraTrainer(){
}

void
MiraTrainer::train(Instance *inst, DecodeResults *results, Parameter *param, double curUpdSeq) {

    int num_results = results->size();
    Items  *items = inst->items();
    Labels *goldLabels = inst->labels();
    int len = items->size();
    double* b = new double[num_results];

    vector<SparseVector> a(num_results, SparseVector());

    double gold_score = calInstScore( items, goldLabels, param);

    // TODO
    // evaluator->LossValue(results, inst, b);
    computeLossValue(results, inst, b);

    for (int i = 0; i < num_results; ++ i) {
        Labels* predLabels = results->at(i);
        double  pred_score  = calInstScore( items, predLabels, param);
        b[i] -= (gold_score - pred_score);

        for (int j = 0; j < items->size(); ++ j) {
            if ( predLabels->at(j) == goldLabels->at(j) ) {
                continue;
            }

            Item* item = items->at(j);
            int goldLabel = goldLabels->at(j);
            int predLabel = predLabels->at(j);

            for (int k = 0; k < item->size(goldLabel); ++ k) {
                a[i].add( item->at(k, goldLabel), 1.0 );
            }
            for (int k = 0; k < item->size(predLabel); ++ k) {
                a[i].add( item->at(k, predLabel), -1.0 );
            }
        }

        for (int j = 0; j < items->size(); ++ j) {
            if (j == 0) {
                a[i].add( m_NumFeatures * m_NumLabels
                        + m_NumLabels * m_NumLabels
                        + goldLabels->at(j), 1.0);

                a[i].add( m_NumFeatures * m_NumLabels
                        + m_NumLabels * m_NumLabels 
                        + predLabels->at(j), -1.0);
            } else {
                a[i].add( m_NumFeatures * m_NumLabels
                        + goldLabels->at(j - 1) * m_NumLabels
                        + goldLabels->at(j), 1.0);

                a[i].add( m_NumFeatures * m_NumLabels
                        + predLabels->at(j - 1) * m_NumLabels
                        + predLabels->at(j), -1.0);
            }
        }

        // b[i] -= score(a[i], param);
    }

    double* alpha = new double[num_results];
    for (int i = 0; i < num_results; ++ i) alpha[i] = 0.0;

    hildreth( a, b, alpha, num_results );

    for (int i = 0; i < num_results; ++ i) {
        for (map<int, double>::iterator itx = a[i].begin();
            itx != a[i].end(); ++ itx) {

                param->update(itx->first, itx->second * alpha[i], curUpdSeq);
        }

    }

    delete[] b;
    delete[] alpha;
}

void
MiraTrainer::computeLossValue(DecodeResults* results, Instance *inst, double *value) {
    Labels *gold = inst->labels();
    for (int i = 0; i < results->size(); ++ i) {
        Labels *result = results->at(i);

        value[i] = 0.0;
        for (int j = 0; j < result->size(); ++ j) {
            if (result->at(j) != gold->at(j))
                value[i] += 1.0;
        }
        // vector<int> statistic = m_Evaluator->statistic(result, inst);
        // value[i] = (statistic[0] + statistic[1] - 2.0 * statistic[6]) * 0.5;
    }
}

double
MiraTrainer :: calInstScore(Items *items, Labels *labels, Parameter *param) {
    double ret = 0.0;

    if (items->size() != labels->size()) {
        return 0.0;
    }
    for (int i = 0; i < items->size(); ++ i) {
        Item *item = items->at(i);
        int  label = labels->at(i);

        for (int j = 0; j < item->size(label); ++ j) {
            int feature = item->at(j, label);
            ret += param->value(feature);
        }
    }

    for (int i = 0; i < items->size(); ++ i) {
        int label = labels->at(i);
        int prevLabel = i == 0 ? m_NumLabels : labels->at(i - 1);

        ret += param->value(m_NumFeatures*m_NumLabels
                + prevLabel * m_NumLabels
                + label);
    }
    return ret;
}


