/*
 *
 *
 *
 *
 *
 *
 */

#ifndef __LTP_TRAINER_H__
#define __LTP_TRAINER_H__

#include "ltp-parameter.h"
#include "ltp-data.h"

namespace ltp {

namespace framework {

/*
 * Class Trainer
 *
 *  The trainer class provide interface for training parameters
 *  with decode results. Different online algorithm inherit
 *  this class an implement the strategy by overriding train
 *  method.
 */
class Trainer {
public:
    Trainer() {}
    virtual ~Trainer() {}

    /*
     * An interface for train parameter with decode result.
     *
     *  @param[in]  inst    the training instance
     *  @param[in]  results the decode results
     *  @param[in]  param   the parameter
     *  @param[in]  curUpdSeq   the update infomation
     */
    virtual void train(Instance* inst, DecodeResults* results,
            Parameter* param, double curUpdSeq) = 0;
};

}

}

#endif  // end for __LTP_TRAINER_H__
