/*
 *
 *
 *
 *
 *
 *
 *
 */
#ifndef __LTP_READER_H__
#define __LTP_READER_H__

#include "ltp-corpus.h"

namespace ltp {

namespace framework {

/*
 * Class Reader
 *
 *  The class reader read the corpus file and store sentence
 *  into RawCorpus. Reader of different data format inherit
 *  from this interface.
 */
/*
 * File stored corpus is a kind of iterable resource, so
 * the coding implement it oritain the iterator pattern.
 */
class Reader {
public:
    Reader() {}
    virtual ~Reader() {}

    /*
     * An interface of opening the file
     *
     *  @param[in]  filename    the filename
     *  @return     int         0, on success
     *                          -1, on failed
     */
    virtual int open(const char *filename) = 0;

    /*
     * An interface of getting next RawSentence
     *
     *
     */
    virtual RawSentence* getNext() = 0;

    /*
     * An interface of judging whether reach the end of file.
     *
     *  @return     bool    true, on not reach the eof
     *                      false, on reach
     */
    virtual bool hasNext() = 0;

    /*
     *
     *
     *
     *
     */
    virtual RawCorpus *read(const char *filename) = 0;
};

}

}

#endif  // end for __LTP_READER_H__
