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

#ifndef __TAGGER_READER_H__
#define __TAGGER_READER_H__

#include "ltp-corpus.h"
#include "ltp-reader.h"

class TaggerReader : public Reader {
public:
    TaggerReader();
    ~TaggerReader();

    int open(const char *filename);
};

#endif  // end for __TAGGER_READER_H__
