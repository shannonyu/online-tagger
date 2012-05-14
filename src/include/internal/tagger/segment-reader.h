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
#ifndef __TAGGER_SEGMENT_READER_H__
#define __TAGGER_SEGMENT_READER_H__

#include "ltp-corpus.h"
#include "ltp-reader.h"
using namespace ltp::framework;

#include <cstdio>

class SegmentReader : public Reader {
public:
    SegmentReader();
    ~SegmentReader();

    /*
     * Open file
     *
     *  @param[in]  filename    The name of file
     */
    int open(const char *filename);

    /*
     * Judge whether reach the end of file.
     *
     *  @return     bool    true if has next,
     *                      false otherwise.
     */
    bool hasNext();

    /*
     *
     *
     */
    RawSentence *getNext();

    /*
     *
     *
     */
    RawCorpus *read(const char *filename);
private:
    FILE *fp;

};

#endif  // end for __TAGGER_SEGMENT_READER_H__
