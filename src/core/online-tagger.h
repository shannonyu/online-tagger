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
#ifndef __ONLINE_TAGGER_H__
#define __ONLINE_TAGGER_H__

#include "ltp-configure.h"
#include "ltp-data.h"
#include "ltp-decoder.h"
#include "ltp-evaluator.h"
#include "ltp-parameter.h"

#include "model.h"
#include "index-builder.h"

class OnlineTagger {
public:
    OnlineTagger(ltp_configure *cfg,
            Decoder *decoder,
            Evaluator *evaluator,
            Parameter *Parameter);
    ~OnlineTagger();

    /*
     * Tag data from the model.
     *
     *  @param  data    the data
     */
    void tag(Data *data);

    /*
     *
     *
     *
     */
    void tag(const char *data_file);

private:
    ltp_configure *cfg;
    Decoder *decoder;
    Evaluator *evaluator;
    Parameter *parameter;
};

#endif  // end for __ONLINE_TAGGER_H__

