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

#ifndef __MIRA_TRAINER_H__
#define __MIRA_TRAINER_H__

#include "ltp-trainer.h"
#include "pos-evaluator.h"
#include "index-builder.h"

using namespace ltp::framework;

class MiraTrainer : public Trainer {
public:
    // TODO
    MiraTrainer(Evaluator *evaluator,
        IndexBuilder *idbuilder) : m_Evaluator(evaluator), m_IdBuilder(idbuilder) {}
    ~MiraTrainer() {}

    void train(Instance *inst,
            DecodeResults *results,
            Parameter *param,
            double curUpdSeq);

private:
    /*
     * Compute loss value accord to the lose 
     *
     *  @param  results[in] the decode result.
     *  @param  inst[in]    the training instance.
     *  @param  value[out]  the lose value
     */
    void computeLossValue(DecodeResults *results,
            Instance *inst,
            double *value);

    /*
     * Calculate score of the instance.
     *
     *  @param  item[in]    the training instance's item
     *  @param  labels[in]  the label result of the item
     *  @param  param[in]   the parameter
     *  @return double      the score under the parameter
     *                      for the instance.
     */
    double calInstScore(Items *items,
            Labels *labels,
            Parameter *param);

private:
    Evaluator    *m_Evaluator;
    IndexBuilder *m_IdBuilder;
};



#endif  // end for __TAGGER_TRAINER_H__
