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

#ifndef __TAGGER_TRAINER_H__
#define __TAGGER_TRAINER_H__

#include "ltp-trainer.h"
#include "postag-evaluator.h"
#include "index-builder.h"

using namespace ltp::framework;

class MiraTrainer : public Trainer {
public:
    MiraTrainer(Evaluator *evaluator,
        IndexBuilder *idbuilder) : m_Evaluator(evaluator), m_IdBuilder(idbuilder) {}
    ~MiraTrainer() {}

    void train(Instance *inst, DecodeResults *results, Parameter *param, double curUpdSeq);

private:
    void computeLossValue(DecodeResults *results, Instance *inst, double *value);
    double calInstScore(Items *items, Labels *labels, Parameter *param);

private:
    Evaluator    *m_Evaluator;
    IndexBuilder *m_IdBuilder;
};

class PerceptronTrainer : public Trainer {
public:
    PerceptronTrainer(IndexBuilder *builder) : m_IdBuilder(builder) {}
    ~PerceptronTrainer() {}

    void train(Instance *inst, DecodeResults *result, Parameter *param, double curUpdSeq);
private:
    IndexBuilder *m_IdBuilder;
};


#endif  // end for __TAGGER_TRAINER_H__
