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
 * An implement of perceptron algorithm to the training process.
 *
 */
#ifndef __PERCEPTRON_TRAINER_H__
#define __PERCEPTRON_TRAINER_H__

#include "ltp-trainer.h"
#include "ltp-data.h"       // Instance *, DecodeResult *
#include "ltp-parameter.h"  // Parameter *
#include "model.h"

using namespace ltp::framework;

class PerceptronTrainer : public Trainer {
public:
    PerceptronTrainer(Model *model);
    ~PerceptronTrainer();

    void train(Instance *inst,
            DecodeResults *result,
            Parameter *param,
            double curUpdSeq);

private:
    Model *m_Model;

    int m_NumFeatures;
    int m_NumLabels;
};

#endif  // end for __PERCEPTRON_TRAINER_H__
