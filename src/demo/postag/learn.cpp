#include "ltp-log.h"
#include "ltp-configure.h"

#include "data.h"
#include "corpus.h"
#include "model.h"
#include "alphabet.h"
#include "decoder.h"
#include "trainer.h"
#include "index-builder.h"

#include "postag-reader.h"
#include "postag-evaluator.h"
#include "postag-extractor.h"

#include "online-learner.h"

/* ----------------------------------------------
 *  Function of main learning process
 * --------------------------------------------- */
int
main_learn(ltp_configure *cfg) {

    Reader *reader = new PostagReader();

    RawCorpus* trainCorpus = reader->read(cfg->config("train").c_str());
    RawCorpus* devCorpus = reader->read(cfg->config("devolop").c_str());
    RawCorpus* testCorpus = reader->read(cfg->config("test").c_str());

    write_log(LTP_LOG_TRACE,
            "Reading corpus is done.");

    Alphabet *featAlpha = new HashDict();
    Alphabet *tagAlpha  = new HashDict();
    Alphabet *itemAlpha = new HashDict();

    Extractor *extractor = new PostagExtractor(
            featAlpha, tagAlpha, itemAlpha);

    Data *trainData = extractor->extract(trainCorpus, true);
    Data *devData   = extractor->extract(devCorpus);
    Data *testData  = extractor->extract(testCorpus);

    write_log(LTP_LOG_TRACE,
            "Extracting featrue is done.");
    write_log(LTP_LOG_TRACE,
            "Features number [%d]", featAlpha->size());
    write_log(LTP_LOG_TRACE,
            "Labels number [%d]", tagAlpha->size());

    DecodeRule *rule = new DecodeRule(itemAlpha, tagAlpha);
    if (rule->load(cfg->config("dict").c_str()) == -1 ) {
        rule = NULL;
    }

    IndexBuilder *idbuilder = new IndexBuilder(featAlpha->size(), tagAlpha->size());

    int agenda = atoi(cfg->config("agenda").c_str());
    Decoder *decoder = new RuleDecoder(rule,
            idbuilder, agenda);

    Evaluator *evaluator = new PostagEvaluator(
            cfg->config("dict").c_str(), itemAlpha);

    Trainer *trainer = NULL;

    if ("MIRA" == cfg->config("algorithm")) {
        trainer = new MiraTrainer( evaluator, idbuilder );
    } else if ("Perceptron" == cfg->config("algorithm")) {
        trainer = new PerceptronTrainer( idbuilder );
    } else {
        return -1;
    }

    Model *model = new Model();

    model->registAlphabet("FEATURES", featAlpha);
    model->registAlphabet("LABELS",   tagAlpha);

    write_log(LTP_LOG_TRACE,
            "Decoder, Trainer, Evaluator is prepared.");

    OnlineLearner *learner = new OnlineLearner(
            cfg, model, decoder, trainer, evaluator);

    learner->learn(trainData, devData, testData);

    // delete rule;
    // delete idbuilder;
    delete decoder;

    delete trainCorpus;
    delete testCorpus;
    delete devCorpus;

    delete trainData;
    delete devData;
    delete testData;

    return 0;
}

