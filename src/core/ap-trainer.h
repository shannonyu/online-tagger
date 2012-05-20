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
#include "index-builder.h"  // TODO

using namespace ltp::framework;

class PerceptronTrainer : public Trainer {
public:
    PerceptronTrainer(IndexBuilder *builder) : m_IdBuilder(builder) {}
    ~PerceptronTrainer() {}

    void train(Instance *inst,
            DecodeResults *result,
            Parameter *param,
            double curUpdSeq);

private:
    IndexBuilder *m_IdBuilder;
};

#endif  // end for __PERCEPTRON_TRAINER_H__
