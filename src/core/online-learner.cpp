#include "online-learner.h"

#include "ltp-log.h"
#include "corpus.h"
#include "hash-alphabet.h"
#include "c-data.h"
#include "c-parameter.h"

#include <sstream>

OnlineLearner::OnlineLearner(ltp_configure *cfg,
        Model *model,
        Decoder *decoder,
        Trainer *trainer,
        Evaluator *evaluator) {

    int handle = 0;
    handle ^= (1<<0);
    handle ^= (1<<1);
    handle ^= (1<<3);

    int numFeatures = model->getAlphabet("FEATURES")->size();
    int numLabels   = model->getAlphabet("LABELS")->size();
    int numParameters = numFeatures * numLabels + numLabels * (numLabels + 1);
    TRACE_LOG("Dimension of parameter %d", numParameters);

    this->globalParam = new CParameter(
            handle, numParameters);

    this->cfg = cfg;
    this->model = model;
    this->decoder = decoder;
    this->trainer = trainer;
    this->evaluator = evaluator;
}

OnlineLearner::OnlineLearner(ltp_configure *cfg,
        Reader *reader,
        Extractor *extractor,
        Decoder *decoder,
        Trainer *trainer,
        Evaluator *evaluator) {

    this->cfg = cfg;
    this->decoder = decoder;
    this->trainer = trainer;
    this->evaluator = evaluator;

    this->model = new Model();
}

OnlineLearner::~OnlineLearner() {
}

void
OnlineLearner::learn(const char *train_file,
        const char *eval_file,
        const char *test_file) {

    RawCorpus *train_crp = reader->read(train_file);
    RawCorpus *eval_crp  = reader->read(eval_file);
    RawCorpus *test_crp  = reader->read(test_file);

    if (NULL == train_crp) {
        WARNING_LOG("Failed to load train corpus.");
        return;
    }
    if (NULL == eval_crp)
        WARNING_LOG("Failed to load eval corpus.");
    if (NULL == test_crp)
        WARNING_LOG("Failed to load test corpus.");

    TRACE_LOG("Reading corpus is done.");
    TRACE_LOG("Read train corpus [%d] line", train_crp->size());

    Alphabet *features = new HashDict();
    Alphabet *labels   = new HashDict();
    Alphabet *chars    = new HashDict();

    Data *train_dat = this->extractor->extract(train_crp, true);
    Data *eval_dat  = this->extractor->extract(eval_crp);
    Data *test_dat  = this->extractor->extract(test_crp);

    if (NULL == train_dat) {
        WARNING_LOG("No training instances is extracted.");
        return;
    }

    this->model->registAlphabet("FEATURES", features);
    this->model->registAlphabet("LABELS", labels);
    int handle = (1<<0)|(1<<1)|(1<<3);
    int numFeatures = model->getAlphabet("FEATURES")->size();
    int numLabels   = model->getAlphabet("LABELS")->size();
    int numParameters = numFeatures * numLabels + numLabels * (numLabels + 1);
    TRACE_LOG("Dimension of parameter %d", numParameters);

    this->globalParam = new CParameter(
            handle, numParameters);

    this->learn(train_dat, eval_dat, test_dat);
}

void
OnlineLearner::learn(Data *train, Data *dev, Data *test) {

    fprintf(stderr, "==========\n");

    int bestModelIndex = -1;
    double bestModelAccuracy = 0.0;
    Parameter *bestModel = NULL;

    int iteration = atoi(cfg->config("itertime").c_str());

    for (int i = 0; i < iteration; ++ i) {
        Parameter *param = learnIter(train, i);
        double accuracy = evaluateIter(dev, test, param, i);
        if (accuracy > bestModelAccuracy || bestModelIndex == -1) {
            bestModelIndex = i;
            bestModelAccuracy = accuracy;
        }
        delete (CParameter *)param;
    }
}

Parameter *
OnlineLearner::learnIter(Data *data, int iter) {
    for (int i = 0; i < data->size(); ++ i) {
        Instance *inst = data->at(i);
        DecodeResults *results = decoder->decode(inst, globalParam);
        double curUpdSeq = (double)data->size() * iter + i;
        trainer->train(inst, results, globalParam, curUpdSeq);
        delete results;

        if((i+1) % 1000 == 0) {
            TRACE_LOG("[%d] instances is trained.", (i+1));
        }
    }

    // Here, it plays a little trick to get the averaged model each iterator.
    // It make a duplicate of the parameter and average it, while the global
    // parameter is not changed at all.
    int handle = 0;
    handle ^= (1<<0);
    handle ^= (1<<1);
    handle ^= (1<<3);

    CParameter *ret = new CParameter(handle, globalParam->size());

    ret->copy((CParameter *)globalParam);

    for (int i = 0; i < ret->size(); ++ i) {
        ret->update(i, 0, (double)(iter + 1) * data->size() - 1);
    }
    ret->averaged((double)(iter + 1) * data->size());
    return (Parameter *)ret;
}

double
OnlineLearner::evaluateIter(Data *dev, Data *test, Parameter *param, int iter) {

    fprintf(stderr, ">> Reporting on iteration (%d)\n", iter + 1);
    fprintf(stderr, ">>> Dev\n");
    evaluator->start();
    for (int i = 0; i < dev->size(); ++ i) {
        Instance *inst = dev->at(i);

        DecodeResults *results = decoder->decode(inst, param);
        if (0 != results->size())
            evaluator->evaluate(results->best(), inst, true);

        delete results;
    }
    evaluator->end();
    evaluator->report();
    double ret = evaluator->p();

    fprintf(stderr, ">>> Test\n");
    evaluator->start();
    for (int i = 0; i < test->size(); ++ i) {
        Instance *inst = test->at(i);

        DecodeResults *results = decoder->decode(inst, param);
        if (0 != results->size())
            evaluator->evaluate(results->best(), inst, true);

        delete results;
    }
    evaluator->end();
    evaluator->report();

    stringstream S; S << iter;
    string modelFile = cfg->config("model") + "." + S.str();
    model->registParameter("PARAMETER", param, true);
    model->saveModel(modelFile.c_str());

    fprintf(stderr, "----------\n");
    return ret;
}
