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

#ifndef __LTP_EXTRACTOR_H__
#define __LTP_EXTRACTOR_H__

#include "ltp-corpus.h"
#include "ltp-data.h"

namespace ltp {

namespace framework {

/*
 * Class Extractor
 *
 *  The class extractor implement of converting raw corpus to data
 *  which is represented by hashed integer.
 *
 *  Extractor is a class work in tagger frontend.
 *
 */

class Extractor {
public:
    Extractor() {}
    virtual ~Extractor() {}

    /*
     * An interface of extract data from corpus.
     *
     *  @param[in]  corpus  the corpus.
     *  @param[in]  append  identify whether the feature can be
     *                      append to the feature.
     *  @return     the data after extract.
     */
    virtual Data *extract(RawCorpus *corpus, bool append = false) = 0;

    /*
     * An interface of extract instance from sentence.
     *
     *  @param[in]  sent    the sentence.
     *  @param[in]  append  identify whether the feature can be
     *                      append to the feature.
     *  @return     the instance.
     */
    virtual Instance *extract(RawSentence *sent, bool append = false) = 0;
};

}

}

#endif  // end for __LTP_EXTRACTOR_H__
