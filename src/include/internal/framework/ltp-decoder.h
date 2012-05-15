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
#ifndef __LTP_DECODER_H__
#define __LTP_DECODER_H__

#include "ltp-data.h"
#include "ltp-parameter.h"

namespace ltp {

namespace framework {

/*
 * Class Decoder
 *
 *  The class decoder define interface of decoding instance with
 *  certain parameter and return the decode results
 */
class Decoder {
public:
    Decoder() {}
    virtual ~Decoder() {}

    /*
     * An interface for decoding the instance with param.
     *
     *  @param[in]  inst    the instance,
     *  @param[in]  param   the paramter,
     *  @return     DecodeResults   the decode result.
     */
    virtual DecodeResults *decode(Instance *inst, Parameter *param) = 0;
};

}  // ltp

}  // framework

#endif  // end for __LTP_DECODER_H__
