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

PerceptronTrainer::PerceptronTrainer(Model *model){
    this->m_Model = model;
    this->m_NumFeatures = model->getAlphabet("FEATURES")->size();
    this->m_NumLabels   = model->getAlphabet("LABELS")->size();
}

PerceptronTrainer::~PerceptronTrainer(){
}

void
PerceptronTrainer::train(Instance *inst, DecodeResults *results, Parameter *param, double curUpdSeq) {

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
            param->update(m_NumFeatures * m_NumLabels
                    + m_NumLabels * m_NumLabels
                    + goldLabels->at(i), 1.0);

            param->update(m_NumFeatures * m_NumLabels
                    + m_NumLabels * m_NumLabels
                    + predLabels->at(i), -1.0);

        } else {
            param->update(m_NumFeatures * m_NumLabels
                    + goldLabels->at(i - 1) * m_NumLabels
                    + goldLabels->at(i), 1.0);

            param->update(m_NumFeatures * m_NumLabels
                    + predLabels->at(i - 1) * m_NumLabels
                    + predLabels->at(i), -1.0);
        }
    }
}
