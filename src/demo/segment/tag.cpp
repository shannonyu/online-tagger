#include "ltp-log.h"
#include "ltp-configure.h"

#include "corpus.h"
#include "hash-alphabet.h"
#include "c-data.h"
#include "c-parameter.h"
#include "seg-decoder.h"
// #include "index-builder.h"

#include "model.h"
#include "seg-reader.h"
#include "seg-evaluator.h"
#include "seg-extractor.h"

#include "online-tagger.h"

int
main_tag(ltp_configure *cfg) {
    Reader *reader = new SegmentReader();

    RawCorpus *corpus = reader->read(cfg->config("devolop").c_str());
    if (NULL == corpus) {
        return -1;
    }
    TRACE_LOG("Reading corpus is done.");

    Alphabet *features = new HashDict();
    Alphabet *labels = new HashDict();
    Alphabet *words = new HashDict();
    Parameter *param = new CParameter(0,0);

    Model *model = new Model();
    model->registAlphabet("FEATURES", features);
    model->registAlphabet("LABELS", labels);
    model->registParameter("PARAMETER", param);
    model->loadModel(cfg->config("model").c_str());
    TRACE_LOG("Loading model is done.");

    const char *dictFile = cfg->config("dict").c_str();
    Extractor *extractor = new SegmentExtractor(dictFile, features, labels, words);
    Data *data = extractor->extract(corpus, false);

    // IndexBuilder *idBuilder = new IndexBuilder(features->size(), labels->size());
    // int agenda = atoi(cfg->config("agenda").c_str());
    Decoder *decoder = new SegmentDecoder(model, 1);
    Evaluator *evaluator = new SegmentEvaluator(dictFile, words, labels);

    for (int i = 0; i < data->size(); ++ i) {
        Instance *inst = data->at(i);
        DecodeResults *results = decoder->decode(inst, param);

        Items *items = inst->items();
        Labels *label = results->best();
        vector<string> ret;
        string tag;
        string word;

        for (int j = 0; j < items->size(); ) {
            tag = labels->rlookup(label->at(j));
            if ("S" == tag) {
                word = corpus->at(i)->at(j)->item();
                ret.push_back(word);
                ++ j;
            } else if ("B" == tag) {
                word = "";
                while ("E" != tag && j < items->size()) {
                    word = word + corpus->at(i)->at(j)->item();
                    tag = labels->rlookup(label->at(j));
                    ++ j;
                }
                ret.push_back(word);
            } else {
                cerr << "Exception catchecd." << endl;
                break;
            }
        }

        for (int j = 0; j < ret.size(); ++ j) {
            if (j == ret.size() - 1) {
                cout << ret[j] << endl;
            } else {
                cout << ret[j] << "\t";
            }
        }

        delete results;
    }
    // OnlineTagger *tagger = new OnlineTagger(cfg, decoder, evaluator, param);
    // tagger->tag(data);

    TRACE_LOG("Tagging is done.");
    return 0;
}
