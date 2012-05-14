#include "ltp-log.h"
#include "ltp-configure.h"

#include "corpus.h"
#include "alphabet.h"
#include "data.h"
#include "parameter.h"
#include "decoder.h"
#include "index-builder.h"

#include "model.h"
#include "segment-reader.h"
#include "segment-evaluator.h"
#include "segment-extractor.h"

#include "online-tagger.h"

int
main_tag(ltp_configure *cfg) {
    Reader *reader = new SegmentReader();

    RawCorpus *corpus = reader->read(cfg->config("devolop").c_str());
    if (NULL == corpus) {
        return -1;
    }
    write_log(LTP_LOG_TRACE,
            "Reading corpus is done.");

    Alphabet *features = new HashDict();
    Alphabet *labels = new HashDict();
    Alphabet *words = new HashDict();
    Parameter *param = new CParameter(0,0);

    Model *model = new Model();
    model->registAlphabet("FEATURES", features);
    model->registAlphabet("LABELS", labels);
    model->registParameter("PARAMETER", param);
    model->loadModel(cfg->config("model").c_str());

    const char *dictFile = cfg->config("dict").c_str();
    write_log(LTP_LOG_TRACE, "");
    Extractor *extractor = new SegmentExtractor(dictFile, features, labels, words);
    Data *data = extractor->extract(corpus, false);

    IndexBuilder *idBuilder = new IndexBuilder(features->size(), labels->size());
    int agenda = atoi(cfg->config("agenda").c_str());
    Decoder *decoder = new PlainDecoder(idBuilder, agenda);
    Evaluator *evaluator = new SegmentEvaluator(dictFile, words, labels);

    OnlineTagger *tagger = new OnlineTagger(cfg, decoder, evaluator, param);
    tagger->tag(data);

    write_log(LTP_LOG_TRACE, "");
    return 0;
}
