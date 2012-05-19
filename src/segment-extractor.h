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
#ifndef __SEGMENT_EXTRACTOR_H__
#define __SEGMENT_EXTRACTOR_H__

#include "ltp-extractor.h"
#include "ltp-alphabet.h"

#include "hash_map.hpp"

#include <iostream>

using namespace std;
using namespace ltp::framework;

class SegmentExtractor : public Extractor {
public:
    SegmentExtractor(
            const char *filename,
            Alphabet *featureDict,
            Alphabet *labelDict,
            Alphabet *charDict);
    ~SegmentExtractor();

    Data     *extract(RawCorpus *corpus, bool append = false);
    Instance *extract(RawSentence *sent, bool append = false);
private:
    Alphabet *m_FeatureDict;
    Alphabet *m_LabelDict;
    Alphabet *m_CharDict;

    hash_set<string> m_Lexicon;
};

#endif  // end for __SENTENCE_EXTRACTOR_H__

