#include "ltp-log.h"
#include "ltp-configure.h"

#include "corpus.h"
#include "alphabet.h"
#include "data.h"
#include "parameter.h"
#include "decoder.h"
#include "index-builder.h"

#include "model.h"
#include "postag-reader.h"
#include "postag-evaluator.h"
#include "postag-extractor.h"

#include "online-tagger.h"

int
main_tag(ltp_configure *cfg) {
    Reader *reader = new PostagReader();

    RawCorpus *corpus = reader->read(cfg->config("devolop").c_str());
    if (NULL == corpus) {
        return -1;
    }
    write_log(LTP_LOG_TRACE,
            "Reading corpus is done.");

    Alphabet *featAlpha = new HashDict();
    Alphabet *tagAlpha  = new HashDict();
    Alphabet *itemAlpha = new HashDict();

    Parameter *param = new CParameter(0,0);

    Model *model = new Model();
    model->registAlphabet("FEATURES", featAlpha);
    model->registAlphabet("LABELS",   tagAlpha);
    model->registParameter("PARAMETER", param);
    if (model->loadModel(cfg->config("model").c_str()) != 0) {
        write_log(LTP_LOG_WARNING, "Failed to load model.");
        return -1;
    }

    write_log(LTP_LOG_TRACE, "Model is loaded.");

    fprintf(stderr, ">> Model Information\n");
    fprintf(stderr, "* Number of features: %d\n", featAlpha->size());
    fprintf(stderr, "* Number of labels: %d\n", tagAlpha->size());
    fprintf(stderr, "* Number of parameters: %d\n", param->size());

    Extractor *extractor = new PostagExtractor(featAlpha, tagAlpha, itemAlpha);
    Data *data = extractor->extract(corpus, false);

    DecodeRule *rule = new DecodeRule(itemAlpha, tagAlpha);
    if (rule->load(cfg->config("dict").c_str()) == -1) {
        rule = NULL;
    }

    IndexBuilder *idBuilder = new IndexBuilder(featAlpha->size(), tagAlpha->size());
    int agenda = atoi(cfg->config("agenda").c_str());
    Decoder *decoder = new RuleDecoder(rule,
            idBuilder, agenda);
    Evaluator *evaluator = new PostagEvaluator(cfg->config("dict").c_str(), itemAlpha);

    evaluator->start();
    for (int i = 0; i < data->size(); ++ i) {
        Instance *inst = data->at(i);

        DecodeResults *results = decoder->decode(inst, param);
        evaluator->evaluate(results->best(), inst, true);

        Items *items = inst->items();
        for (int j = 0; j < items->size(); ++ j) {
            fprintf(stderr, "%s_%s",
                    itemAlpha->rlookup(items->at(j)->item()),
                    tagAlpha->rlookup(results->best()->at(j)));
            fprintf(stdout, "%c", j == items->size() - 1 ? '\n' : ' ');
        }
        delete results;
    }
    evaluator->end();
    evaluator->report();

    write_log(LTP_LOG_TRACE, "Decode is done.");
    return 0;
}
