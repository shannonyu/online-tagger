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
#ifndef __POS_DECODER_H__
#define __POS_DECODER_H__

#include "ltp-decoder.h"
#include "ltp-alphabet.h"

#include "model.h"
#include "ltp-math-const.h"

using namespace ltp::math;
using namespace ltp::framework;

class DecodeRule {
public:
    DecodeRule(Alphabet *words, Alphabet *labels);
    ~DecodeRule();

    int load(const char *filename);
    bool legal(int condition, int tag);
private:
    Alphabet *m_WordDict;
    Alphabet *m_LabelDict;
    bool *m_LegalMat;
};

class RuleDecoder : public Decoder {
public:
    RuleDecoder(DecodeRule *rule,
            Model *model,
            int agenda);
    ~RuleDecoder();

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
    int        m_Agenda;
    Model      *m_Model;
    DecodeRule *m_Rule;
};

#endif  // end for __POS_DECODE_H__
