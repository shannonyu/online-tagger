#include "otpos_dll.h"

#include "ltp-log.h"
#include "ltp-configure.h"

#include "corpus.h"
#include "hash-alphabet.h"
#include "c-data.h"
#include "c-parameter.h"
#include "pos-decoder.h"

#include "model.h"
#include "pos-extractor.h"

#include "online-tagger.h"

struct OTPOS_Engine {
    Model     *model;
    Decoder   *decoder;
    Extractor *extractor;
};

otpos_t OTPOS_Load(const char *cfg_file) {

    // allocate config parser and set default config.
    ltp_configure *cfg = new ltp_configure();

    cfg->set_cfg("target", "test");
    cfg->set_cfg("agenda", "1");
    cfg->set_cfg("model", "./");
    cfg->set_cfg("dict", "./");

    // load config.
    if (-1 == cfg->load_cfg(cfg_file)) {
        WARNING_LOG("Failed to load config file");
        delete cfg;
        return NULL;
    }

    OTPOS_Engine *engine = new OTPOS_Engine();

    // allocate alphabet
    Alphabet *features = new HashDict();
    Alphabet *labels   = new HashDict();
    Alphabet *words    = new HashDict();
    Parameter *param   = new CParameter(0, 0);

    // load model.
    Model *model       = new Model();
    model->registAlphabet("FEATURES", features);
    model->registAlphabet("LABELS",   labels);
    model->registAlphabet("WORDS",    words);
    model->registParameter("PARAMETER", param);
    model->loadModel(cfg->config("model").c_str());

    TRACE_LOG("Loading model is done.");
    TRACE_LOG("Num Features: %d", features->size());
    TRACE_LOG("Num Labels: %d", labels->size());

    Extractor *extractor = new PostagExtractor(
            features, labels, words);
    DecodeRule *rule = new DecodeRule(words, labels);
    if (-1 == rule->load(cfg->config("dict").c_str()))
        rule = NULL;

    Decoder *decoder = new RuleDecoder(rule, model, 1);

    engine->model = model;
    engine->extractor = extractor;
    engine->decoder = decoder;

    return reinterpret_cast<otpos_t>(engine);
}

int OTPOS_Destroy(otpos_t handle) {
    return 0;
}

int OTPOS_Postag_x(otpos_t handle,
        const std::vector<std::string>& sent,
        std::vector<std::string>& pos) {

    OTPOS_Engine *engine = reinterpret_cast<OTPOS_Engine *>(handle);

    pos.clear();
    RawSentence *tag_sent = new TagSent();
    for (int i = 0; i < sent.size(); ++ i) {
        tag_sent->append(new TagItem(sent[i], "_"));
    }

    Instance *inst = engine->extractor->extract(tag_sent, false);
    Items *items = inst->items();
    Labels *labels = engine->decoder->decode(inst,
            engine->model->getParameter("PARAMETER"))->best();
    Alphabet *labelsDict = engine->model->getAlphabet("LABELS");
    pos.resize(labels->size());
    for (int i = 0; i < labels->size(); ++ i) {
        pos[i] = labelsDict->rlookup(labels->at(i));
        cerr << pos[i] << " | ";
    }
    cerr << endl;

    return pos.size();
}
