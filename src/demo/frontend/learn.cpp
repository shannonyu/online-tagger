#include "ltp-argtable.h"
#include "ltp-configure.h"

#include "alphabet.h"
#include "data.h"
#include "index-builder.h"
#include "decoder.h"
#include "model.h"
#include "trainer.h"

#include "tagger-evaluator.h"
#include "online-learner.h"

#include "cppstrlib.h"
using namespace ltp::utility;

#include <iostream>
#include <sstream>
using namespace std;

void
init_learn_opt(ltp_configure *cfg) {
    cfg->set_cfg("train",  "");
    cfg->set_cfg("dev",    "");
    cfg->set_cfg("eval",   "");
    cfg->set_cfg("dict",   "");
    cfg->set_cfg("model",  "");
    cfg->set_cfg("iter",   "0");
    cfg->set_cfg("agenda", "0");
}

int
parse_opt(ltp_configure *cfg, ltp_argtable *argtable) {
    void *tmp = NULL;

    // train file
    if(!(tmp = argtable->option("train"))) {
        return -1;
    } else {
        cfg->set_cfg("train", *(const char **)tmp);
    }

    // devolop file
    if(!(tmp = argtable->option("dev"))) {
        return -1;
    } else {
        cfg->set_cfg("dev", *(const char **)tmp);
    }

    // evaluate file
    if(!(tmp = argtable->option("eval"))) {
        return -1;
    } else {
        cfg->set_cfg("eval", *(const char **)tmp);
    }

    // model file
    if(!(tmp = argtable->option("model"))) {
        return -1;
    } else {
        cfg->set_cfg("model", *(const char **)tmp);
    }

    // iteration time
    if(!(tmp = argtable->option("iter"))) {
        cfg->set_cfg("iter", "10");
    } else {
        stringstream S; S << *(int *)tmp;
        cfg->set_cfg("iter", S.str().c_str());
    }

    // num of agenda
    if(!(tmp = argtable->option("agenda"))) {
        cfg->set_cfg("agenda", "5");
    } else {
        stringstream S; S << *(int *)tmp;
        cfg->set_cfg("agenda", S.str().c_str());
    }

    return 0;
}

/*
 * Function to build data from file.
 *
 *  @param[out]     data
 *  @param[in]      filename
 *  @param[in/out]  features
 *  @param[in/out]  labels
 */
int
build_data(Data *data,
        const char *filename,
        Alphabet *featureDict,
        Alphabet *labelDict,
        bool append=false) {

    FILE *fp = NULL;
    fp = fopen(filename, "r");

    if(NULL == fp) {
        fprintf(stderr, "ERROR: Failed to open file (%s)\n", filename);
        return -1;
    }

    char buff[10005];
    bool eof = false;
    while(false == eof) {
        Items *items = new CItems();
        Labels *labels = new CLabels();
        while(true) {
            eof = (fgets(buff, 10005, fp) == NULL);

            if(eof) {
                break;
            }

            string line = cppstr::chomp(string(buff));
            if (0 == line.length()) {
                break;
            }

            vector<string> sep = cppstr::split(line);
            labels->append(labelDict->insert(sep[0]));

            Item *item = new CItem();
            for (int i = 1; i < sep.size(); ++ i) {
                if(append) {
                    item->append(featureDict->insert(sep[i]));
                } else {
                    int idx = -1;
                    if(-1 != (idx = featureDict->lookup(sep[i])))
                        item->append(idx);
                }
            }

            items->append(item);
        }

        data->append(new CInstance(items, labels));
    }

    return 0;
}

int
main_learn(int argc, char **argv, ltp_argtable *argtable) {
    ltp_configure cfg;
    init_learn_opt(&cfg);

    if(0 != argtable->parse_args(argc, (const char **)argv)) {
        fprintf(stderr, "EROOR: Failed to parse arguments.\n");
        return -1;
    }

    if(0 != parse_opt(&cfg, argtable)) {
        fprintf(stderr, "ERROR: Not enough argument.\n");
        return -1;
    }

    Alphabet *features = new HashDict();
    Alphabet *labels   = new HashDict();

    Data *trainData = new CData();
    Data *devData   = new CData();
    Data *evalData  = new CData();

    build_data(trainData, cfg.config("train").c_str(), features, labels, true);
    build_data(devData,   cfg.config("dev").c_str(),   features, labels, false);
    build_data(evalData,  cfg.config("eval").c_str(),  features, labels, false);

    int agenda = cppstr::toInt(cfg.config("agenda"));

    IndexBuilder *idBuilder = new IndexBuilder(features->size(), labels->size());
    Decoder *decoder = new PlainDecoder(idBuilder, agenda);
    Evaluator *evaluator = new TaggerEvaluator();
    Trainer *trainer = new MiraTrainer(NULL, idBuilder);
    Model *model = new Model();

    model->registAlphabet("FEATURES", features);
    model->registAlphabet("LABELS", labels);

    OnlineLearner *learner = new OnlineLearner(
            &cfg, model, decoder, trainer, evaluator);
    learner->learn(trainData, devData, evalData);

    delete idBuilder;
    delete decoder;
    delete trainer;
    delete trainData;
    delete devData;
    delete evalData;

    return 0;
}
