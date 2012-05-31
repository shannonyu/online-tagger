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

#ifndef __TAGGER_POSTAG_EXTRACTOR_H__
#define __TAGGER_POSTAG_EXTRACTOR_H__

#include "ltp-extractor.h"
#include "ltp-alphabet.h"
using namespace ltp::framework;

class PostagExtractor : public Extractor {
public:
    PostagExtractor(
            Alphabet *featureDict,
            Alphabet *labelDict,
            Alphabet *wordDict);

    ~PostagExtractor();

    /*
     *
     *
     *
     */
    Data *extract(RawCorpus *corpus, bool append = false);

    /*
     *
     *
     *
     *
     */
    Instance *extract(RawSentence *sent, bool append = false);
private:
    Alphabet *m_FeatureDict;
    Alphabet *m_LabelDict;
    Alphabet *m_WordDict;
};

#endif  // end for __TAGGER_POSTAG_EXTRACTOR_H__
