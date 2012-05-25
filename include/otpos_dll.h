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
#ifndef __OTPOS_DLL_H__
#define __OTPOS_DLL_H__

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

OTPOS_Engine *OTPOS_Load(const char *cfg);
int OTPOS_Destroy(OTPOS_Engine *engine);
int OTPOS_Postag_x(OTPOS_Engine *engine,
        const std::vector<std::string>& sent,
        std::vector<std::string>& pos);

#endif  // end for __OTPOS_DLL_H__
