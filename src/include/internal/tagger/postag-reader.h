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
#ifndef __TAGGER_POSTAG_READER_H__
#define __TAGGER_POSTAG_READER_H__

#include "ltp-corpus.h"
#include "ltp-reader.h"

#include <cstdio>
using namespace ltp::framework;

class PostagReader : public Reader {
public:
    PostagReader();
    ~PostagReader();

    /*
     * Open file
     *
     *  @param[in]  filename    The name of file
     */
    int open( const char *filename);

    /*
     * Judge whether reach the end of file.
     *
     *  @return     bool    true if has next,
     *                      false otherwise.
     */
    bool hasNext();

    /*
     * Read a raw sentence
     *
     *  @return RawSentence * the pointer to new sentence,
     *          NULL, on failure.
     */
    RawSentence *getNext();

    /*
     * Read corpus from file
     *
     *  @param[in]  filename    the filename
     *  @return     RawCorpus * the pointer to the corpus
     *              on success,
     *              NULL, otherwise.
     */
    RawCorpus *read(const char *filename);

private:
    FILE *fp;
};

#endif  // end for __TAGGER_POSTAG_READER_H__
