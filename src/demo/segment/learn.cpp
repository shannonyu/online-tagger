#include "ltp-log.h"
#include "ltp-configure.h"

#include "c-data.h"
#include "corpus.h"
#include "model.h"
#include "hash-alphabet.h"
#include "seg-decoder.h"
#include "ap-trainer.h"
#include "mira-trainer.h"
// #include "index-builder.h"

#include "seg-reader.h"
#include "seg-evaluator.h"
#include "seg-extractor.h"

#include "online-learner.h"

/* ----------------------------------------------
 *  Functions for preload labels. To avoid some
 *  inconsistence cause by the labels in small
 *  data sample.
 * -------------------------------------------- */
void preload_Labels(Alphabet *labels) {
    string tag[] = {"B", "M", "E", "S"};
    for (int i = 0; i < 4; ++ i) {
        labels->insert(tag[i]);
    }
}

/* ----------------------------------------------
 *  Function of main learning process
 * --------------------------------------------- */
int
main_learn(ltp_configure *cfg) {

    fprintf(stderr, "==========\n");
    /*  Process of reading */
    Reader *reader = new SegmentReader();

    RawCorpus* trainCorpus = reader->read(cfg->config("train").c_str());
    RawCorpus* devCorpus = reader->read(cfg->config("devolop").c_str());
    RawCorpus* testCorpus = reader->read(cfg->config("test").c_str());

    if (NULL == trainCorpus) {
        WARNING_LOG("Failed to load train corpus.");
        return -1;
    }
    if (NULL == devCorpus)
        WARNING_LOG("Failed to load dev corpus.");
    if (NULL == testCorpus)
        WARNING_LOG("Failed to load test corpus");

    TRACE_LOG("Reading corpus is done.");
    TRACE_LOG("Read train corpus [%d] line", trainCorpus->size());

    Alphabet *features = new HashDict();
    Alphabet *labels   = new HashDict();
    Alphabet *chars    = new HashDict();

    preload_Labels(labels);

    Extractor *extractor = new SegmentExtractor(
            cfg->config("dict").c_str(),
            features, labels, chars);

    Data* trainData = extractor->extract(trainCorpus, true);
    Data* devData = extractor->extract(devCorpus);
    Data* testData = extractor->extract(testCorpus);

    if (NULL == trainData) {
        WARNING_LOG("No training instances is loaded.");
        return -1;
    }

    delete trainCorpus;
    delete testCorpus;
    delete devCorpus;

    TRACE_LOG("Extracting feature is done.");
    TRACE_LOG("Num feat: %d", features->size());
    TRACE_LOG("Num label: %d", labels->size());
    TRACE_LOG("Num forms: %d", chars->size());

    Model *model = new Model();

    model->registAlphabet("FEATURES", features);
    model->registAlphabet("LABELS", labels);

    // IndexBuilder *idbuilder = new IndexBuilder(features->size(), labels->size());

    int agenda = atoi(cfg->config("agenda").c_str());

    Decoder *decoder = new SegmentDecoder(model, agenda);
    Evaluator *evaluator = new SegmentEvaluator(
            cfg->config("dict").c_str(), chars, labels);

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

    // delete idbuilder;
    delete decoder;
    delete trainData;
    delete devData;
    delete testData;

    return 0;
}

