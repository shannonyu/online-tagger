/*
 *
 *
 *
 *
 *
 *
 *
 *
 */
#include "otws_dll.h"

#include "ltp-log.h"
#include "ltp-configure.h"

#include "corpus.h"
#include "hash-alphabet.h"
#include "c-data.h"
#include "c-parameter.h"
#include "seg-decoder.h"

#include "model.h"
#include "seg-reader.h"
#include "seg-evaluator.h"
#include "seg-extractor.h"
#include "cppstrlib.h"
#include "utf.h"

struct OTWS_Engine {
    Model     *model;
    Decoder   *decoder;
    Extractor *extractor;
};

otws_t OTWS_Load(const char *cfg_file) {

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

    OTWS_Engine *engine = new OTWS_Engine();

    // allocate alphabet
    Alphabet *features = new HashDict();
    Alphabet *labels   = new HashDict();
    Alphabet *words    = new HashDict();
    Parameter *param   = new CParameter(0, 0);

    // load model.
    Model *model       = new Model();
    model->registAlphabet("FEATURES", features);
    model->registAlphabet("LABELS", labels);
    model->registParameter("PARAMETER", param);
    model->loadModel(cfg->config("model").c_str());

    TRACE_LOG("Loading model is done.");
    TRACE_LOG("Num Features: %d", features->size());
    TRACE_LOG("Num Labels: %d", labels->size());

    Extractor *extractor = new SegmentExtractor(
            cfg->config("dict").c_str(),
            features, labels, words);

    Decoder *decoder = new SegmentDecoder(model, 1);

    engine->model = model;
    engine->extractor = extractor;
    engine->decoder = decoder;

    return reinterpret_cast<otws_t>(engine);
}

int OTWS_Destroy(otws_t handle) {

    OTWS_Engine *engine = reinterpret_cast<OTWS_Engine *>(handle);
    if (engine->model)
        delete (engine)->model;

    if (engine->extractor)
        delete (engine)->extractor;

    if (engine->decoder)
        delete (engine)->decoder;

    return 0;
}

int OTWS_Wordseg(otws_t engine,
        const char *sent,
        char **words) {

//    Alphabet *labels = engine->model->getAlphabet("LABELS");
    return 0;
}

int OTWS_Wordseg_x(otws_t handle, 
        const string& sent,
        vector<string>& words) {

    OTWS_Engine *engine = reinterpret_cast<OTWS_Engine *>(handle);
    words.clear();

    RawSentence *tag_sent = new TagSent();
    vector<string> chars;
    int numChars = UTF::getCharactersFromUTF8String(sent, &chars);

    // something for debug.
    cerr << "TOKEN: ";
    for (int i = 0; i < numChars; ++ i) {
        cerr << chars[i] << " | ";
        tag_sent->append(new TagItem(chars[i], "X"));
    }
    cerr << endl;

    Instance *inst = (engine)->extractor->extract(tag_sent, false);
    Items *items = inst->items();

    Labels* labels = engine->decoder->decode(inst,
            engine->model->getParameter("PARAMETER"))->best();

    Alphabet *labelsDict = engine->model->getAlphabet("LABELS");

    // something for debug.
    for (int i = 0; i < labels->size(); ++ i) {
        cerr << labelsDict->rlookup(labels->at(i)) << 
            "(" << labels->at(i) << ") ";
    }
    cerr << "| label size: " << labels->size() << endl;

    string tag;
    string word;

    for (int i = 0; i < items->size(); ) {
        tag = labelsDict->rlookup(labels->at(i));

        if ("S" == tag) {
            word = chars[i];
            words.push_back(word);
            ++ i;
        } else if ("B" == tag) {
            word = "";
            while ("E" != tag && i < items->size()){
                word = word + chars[i];
                tag = labelsDict->rlookup(labels->at(i));
                ++ i;
            }
            words.push_back(word);
        } else {
            cerr << "Exception asserted." << endl;
            words.clear();
            return -1;
        }
    }

    return words.size();
}
