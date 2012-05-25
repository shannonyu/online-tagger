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
#ifndef __OT_WORDSEG_DLL_H__
#define __OT_WORDSEG_DLL_H__

#include <string>
#include <vector>
using namespace std;

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

OTWS_Engine *OTWS_Load(const char *cfg_file);
int OTWS_Destroy(OTWS_Engine *engine);
// int OTWS_Wordseg(OTWS_Engine *engine, const char *sent, char **words);
int OTWS_Wordseg_x(OTWS_Engine *engine, const string& sent, vector<string>& words);

#endif  // end for __OT_WORDSEG_DLL_H__
