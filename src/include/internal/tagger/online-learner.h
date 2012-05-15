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
#ifndef __TAGGER_ONLINE_LEARNER_H__
#define __TAGGER_ONLINE_LEARNER_H__

#include "ltp-data.h"
#include "ltp-parameter.h"
#include "ltp-decoder.h"
#include "ltp-trainer.h"
#include "ltp-evaluator.h"

#include "model.h"
#include "index-builder.h"
#include "ltp-configure.h"

class OnlineLearner {
public:
    OnlineLearner(ltp_configure *cfg,
            Model *model,
            Decoder *decoder,
            Trainer *trainer,
            Evaluator *evaluator);
    ~OnlineLearner();

    /*
     * Learn model from the data
     *
     *  @param[in]      data        the data,
     *  @param[in/out]  globalParam the global parameter.
     */
    void learn(Data *train, Data *dev, Data *test);

private:
    /*
     * Method for learn one iteration
     *
     *  @param[in]  data    the data
     *  @param[in]  iter    the iteration
     *  @return     Parameter   the learned parameter of the
     *                          iteration.
     */
    Parameter *learnIter(Data *data, int iter);

    /*
     * Method for evaluate one iteration
     *
     *  @param[in]  dev     the devolop data
     *  @param[in]  test    the test data
     *  @param[in]  param   the parameter.
     */
    double evaluateIter(Data *dev, Data *test, Parameter *param, int iter);

private:
    ltp_configure *cfg;
    Model     *model;
    Decoder   *decoder;
    Trainer   *trainer;
    Evaluator *evaluator;
    Parameter *globalParam;
};

#endif  // end for __ONLINE_LEARNER_H__
