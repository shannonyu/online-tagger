/*
 *
 *
 *
 *
 *
 *
 */

#include "trainer.h"
#include "data.h"

#include "ltp-math.h"
using namespace ltp::math;

void
MiraTrainer :: train(Instance *inst, DecodeResults *results, Parameter *param, double curUpdSeq) {

    int num_results = results->size();
    Items  *items = inst->items();
    Labels *goldLabels = inst->labels();
    int len = items->size();
    double* b = new double[num_results];

    vector<SparseVector> a(num_results, SparseVector());

    double gold_score = calInstScore( items, goldLabels, param);

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
                a[i].add( m_IdBuilder->index( m_IdBuilder->numLabels(), goldLabels->at(j), true ), 1.0);
                a[i].add( m_IdBuilder->index( m_IdBuilder->numLabels(), predLabels->at(j), true ), -1.0);
            } else {
                a[i].add( m_IdBuilder->index( goldLabels->at(j - 1), goldLabels->at(j), true ), 1.0);
                a[i].add( m_IdBuilder->index( predLabels->at(j - 1), predLabels->at(j), true ), -1.0);
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
MiraTrainer :: computeLossValue(DecodeResults* results, Instance *inst, double *value) {
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
        int prevLabel = i == 0 ? m_IdBuilder->numLabels() : labels->at(i - 1);

        ret += param->value( m_IdBuilder->index(prevLabel, label, true) );
    }
    return ret;
}

void
PerceptronTrainer :: train(Instance *inst, DecodeResults *results, Parameter *param, double curUpdSeq) {

    Labels *goldLabels = inst->labels();
    Labels *predLabels = results->best();

    Items *items = inst->items();
    int len = goldLabels->size();
    int key = -1;

    for (int i = 0; i < len; ++ i) {
        if (goldLabels->at(i) == predLabels->at(i)) {
            continue;
        }

        Item *item = items->at(i);
        int goldLabel = goldLabels->at(i);
        int predLabel = predLabels->at(i);

        for (int j = 0; j < item->size(goldLabel); ++ j) {
            key = item->at(j, goldLabel);
            param->update(key, 1.0);
        }

        for (int j = 0; j < item->size(predLabel); ++ j) {
            key = item->at(j, predLabel);
            param->update(key, -1.0);
        }
    }

    for (int i = 0; i < len; ++ i) {
        if (i == 0) {
            key = m_IdBuilder->index( m_IdBuilder->numLabels(), goldLabels->at(i), true );
            param->update( key, 1.0 );

            key = m_IdBuilder->index( m_IdBuilder->numLabels(), predLabels->at(i), true );
            param->update( key, -1.0 );
        } else {
            key = m_IdBuilder->index( goldLabels->at(i - 1), goldLabels->at(i), true );
            param->update( key, 1.0 );

            key = m_IdBuilder->index( predLabels->at(i - 1), predLabels->at(i), true );
            param->update( key, -1.0 );
        }
    }
}
