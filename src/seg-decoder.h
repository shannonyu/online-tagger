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
#ifndef __SEG_DECODER_H__
#define __SEG_DECODER_H__

#include "ltp-decoder.h"
#include "ltp-alphabet.h"

#include "index-builder.h"
#include "ltp-math-const.h"

using namespace ltp::math;
using namespace ltp::framework;

class SegmentDecoder : public Decoder {
public:
    SegmentDecoder(IndexBuilder *builder,
            int agenda);
    ~SegmentDecoder();

    DecodeResults *decode(Instance *inst, Parameter *param);

private:
    struct DecodeState {
        DecodeState(): label(-1), score(NINF), prev(NULL) {}
        DecodeState(int _label,
                double _score,
                DecodeState *_prev) : label(_label), score(_score), prev(_prev) {}
        ~DecodeState() {}

        bool operator < (const DecodeState& T) const {
            return score < T.score - EPS;
        }

        bool operator > (const DecodeState& T) const {
            return score > T.score + EPS;
        }

        bool operator == (const DecodeState& T) const {
            return score >= T.score - EPS && score <= T.score + EPS;
        }

        int label;
        double score;
        DecodeState *prev;
    };

private:
    int m_Agenda;
    IndexBuilder *m_IdBuilder;
};

#endif  // end for __SEG_DECODER_H__
