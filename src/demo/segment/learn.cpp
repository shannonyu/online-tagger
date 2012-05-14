#include "ltp-log.h"
#include "ltp-configure.h"

#include "data.h"
#include "corpus.h"
#include "model.h"
#include "alphabet.h"
#include "decoder.h"
#include "trainer.h"
#include "index-builder.h"

#include "segment-reader.h"
#include "segment-evaluator.h"
#include "segment-extractor.h"

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

    Reader *reader = new SegmentReader();

    RawCorpus* trainCorpus = reader->read(cfg->config("train").c_str());
    RawCorpus* devCorpus = reader->read(cfg->config("devolop").c_str());
    RawCorpus* testCorpus = reader->read(cfg->config("test").c_str());

    if (NULL == trainCorpus) {
        write_log(LTP_LOG_WARNING,
                "Failed to load corpus.");
        return -1;
    }

    write_log(LTP_LOG_TRACE,
            "Reading corpus is done.");
    write_log(LTP_LOG_TRACE,
            "Read train corpus [%d] line", trainCorpus->size());

    Alphabet *features = new HashDict();
    Alphabet *labels   = new HashDict();
    Alphabet *chars    = new HashDict();
    preload_Labels(labels);

    Extractor *extractor = new SegmentExtractor(
            cfg->config("dict").c_str(),
            features,
            labels,
            chars);

    Data* trainData = extractor->extract(trainCorpus, true);
    write_log(LTP_LOG_TRACE, "train data loaded.");
    Data* devData = extractor->extract(devCorpus);
    write_log(LTP_LOG_TRACE, "dev data loaded.");
    Data* testData = extractor->extract(testCorpus);
    write_log(LTP_LOG_TRACE, "test data loaded.");

    write_log(LTP_LOG_TRACE,
            "Extracting featrue is done.");

    IndexBuilder *idbuilder = new IndexBuilder(features->size(), labels->size());

    int agenda = atoi(cfg->config("agenda").c_str());
    Decoder *decoder = new PlainDecoder(idbuilder, agenda);

    Evaluator *evaluator = new SegmentEvaluator(
            cfg->config("dict").c_str(), chars, labels);

    Trainer *trainer = NULL;

    if ("MIRA" == cfg->config("algorithm")) {
        trainer = new MiraTrainer( evaluator, idbuilder );
    } else if ("Perceptron" == cfg->config("algorithm")) {
        trainer = new PerceptronTrainer( idbuilder );
    } else {
        return -1;
    }

    Model *model = new Model();

    model->registAlphabet("FEATURES", features);
    model->registAlphabet("LABELS", labels);

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

