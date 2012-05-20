#include "online-tagger.h"

#include "parameter.h"
#include "ltp-log.h"

OnlineTagger :: OnlineTagger(ltp_configure *cfg,
        Decoder *decoder,
        Evaluator *evaluator,
        Parameter *parameter) {

    this->cfg = cfg;
    this->decoder   = decoder;
    this->evaluator = evaluator;
    this->parameter = parameter;
}

OnlineTagger :: ~OnlineTagger() {
}

void
OnlineTagger :: tag(Data *data) {

    this->evaluator->start();
    for (int i = 0; i < data->size(); ++ i) {
        Instance *inst = data->at(i);

        DecodeResults *results = decoder->decode(inst, parameter);
        evaluator->evaluate(results->best(), inst, true);
        delete results;
    }

    this->evaluator->end();
    this->evaluator->report();
}
