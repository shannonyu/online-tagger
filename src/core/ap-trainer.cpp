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
#include "ap-trainer.h"

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
