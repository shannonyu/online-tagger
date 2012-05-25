#include "ltp-log.h"
#include "ltp-configure.h"

#include "c-data.h"
#include "corpus.h"
#include "model.h"
#include "hash-alphabet.h"
#include "pos-decoder.h"
#include "mira-trainer.h"
#include "ap-trainer.h"
// #include "index-builder.h"

#include "pos-reader.h"
#include "pos-evaluator.h"
#include "pos-extractor.h"

#include "online-learner.h"

/* ----------------------------------------------
 *  Function of main learning process
 * --------------------------------------------- */
int
main_learn(ltp_configure *cfg) {

    fprintf(stderr, "==========\n");
    Reader *reader = new PostagReader();

    RawCorpus* trainCorpus = reader->read(cfg->config("train").c_str());
    RawCorpus* devCorpus = reader->read(cfg->config("devolop").c_str());
    RawCorpus* testCorpus = reader->read(cfg->config("test").c_str());

    TRACE_LOG("Reading corpus is done.");

    Alphabet *featAlpha = new HashDict();
    Alphabet *tagAlpha  = new HashDict();
    Alphabet *itemAlpha = new HashDict();

    Extractor *extractor = new PostagExtractor(
            featAlpha, tagAlpha, itemAlpha);

    Data *trainData = extractor->extract(trainCorpus, true);
    Data *devData   = extractor->extract(devCorpus);
    Data *testData  = extractor->extract(testCorpus);

    delete trainCorpus;
    delete devCorpus;
    delete testCorpus;

    TRACE_LOG("Extracting feature is done.");
    TRACE_LOG("Num feat: %d", featAlpha->size());
    TRACE_LOG("Num label: %d", tagAlpha->size());
    TRACE_LOG("Num forms: %d", itemAlpha->size());

    Model *model = new Model();

    model->registAlphabet("FEATURES", featAlpha);
    model->registAlphabet("LABELS",   tagAlpha);
    model->registAlphabet("WORDS",    itemAlpha);
    DecodeRule *rule = new DecodeRule(itemAlpha, tagAlpha);
    if (rule->load(cfg->config("dict").c_str()) == -1 ) {
        rule = NULL;
    }

    // IndexBuilder *idbuilder = new IndexBuilder(featAlpha->size(), tagAlpha->size());

    int agenda = atoi(cfg->config("agenda").c_str());
    //Decoder *decoder = new RuleDecoder(rule, idbuilder, agenda);
    Decoder *decoder = new RuleDecoder(rule, model, agenda);

    Evaluator *evaluator = new PostagEvaluator(
            cfg->config("dict").c_str(), itemAlpha);

    Trainer *trainer = NULL;

    if ("MIRA" == cfg->config("algorithm")) {
        trainer = new MiraTrainer( evaluator, model );
    } else if ("Perceptron" == cfg->config("algorithm")) {
        trainer = new PerceptronTrainer( model );
    } else {
        return -1;
    }

    OnlineLearner *learner = new OnlineLearner(
            cfg, model, decoder, trainer, evaluator);

    learner->learn(trainData, devData, testData);

    // delete rule;
    // delete idbuilder;
    delete decoder;
    delete trainData;
    delete devData;
    delete testData;

    return 0;
}

