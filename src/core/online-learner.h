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
#include "ltp-reader.h"
#include "ltp-extractor.h"
#include "ltp-parameter.h"
#include "ltp-decoder.h"
#include "ltp-trainer.h"
#include "ltp-evaluator.h"

#include "model.h"
// #include "index-builder.h"
#include "ltp-configure.h"

using namespace ltp::framework;

/*
 * General layer of online-tagger framework:
 *
 *  sentence -> extraction -> core
 *
 * OnlineLearner
 *
 *
 */
class OnlineLearner {
public:

    /*
     * Allocator of (extration, core) interface
     *
     *  @param[in]
     *
     */
    OnlineLearner(ltp_configure *cfg,
            Model *model,
            Decoder *decoder,
            Trainer *trainer,
            Evaluator *evaluator);

    /*
     * Allocator of (sentence, extraction) interface
     *
     *  @param[in]
     *
     */
    OnlineLearner(ltp_configure *cfg,
            Reader *reader,
            Extractor *extractor,
            Decoder *decoder,
            Trainer *trainer,
            Evaluator *evaluator);

    ~OnlineLearner();

    /*
     * Learn model from the corpus.
     *
     *  @param[in]  train   the training file
     *  @param[in]  eval    the evaluation file
     *  @param[in]  test    the testing file
     */
    void learn(const char *train_file,
            const char *eval_file,
            const char *test_file);

    /*
     * Learn model from the data
     *
     *  @param[in]  train   the training data
     *  @param[in]  eval    the evaluation data
     *  @param[in]  test    the testing data
     */
    void learn(Data *train, Data *eval, Data *test);

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
    Reader    *reader;
    Decoder   *decoder;
    Trainer   *trainer;
    Extractor *extractor;
    Evaluator *evaluator;
    Parameter *globalParam;
};

#endif  // end for __ONLINE_LEARNER_H__
