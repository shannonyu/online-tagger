/*
 *
 *
 *
 *
 *
 *
 *
 */
#ifndef __LTP_CORPUS_H__
#define __LTP_CORPUS_H__

namespace ltp {

namespace framework {

class RawItem {
public:
    RawItem() {}
    virtual ~RawItem() {}

    /*
     * An interface for accessing the item
     *
     *  @return the item.
     */
    virtual const char *item() = 0;

    /*
     * An interface for accessing the tag
     *
     *  @return the tag.
     */
    virtual const char *tag() = 0;
};

class RawSentence {
public:
    RawSentence() {}
    virtual ~RawSentence() {}

    /*
     * An interface for append an raw item to sentence.
     *
     *  @param[in]  item    the pointer to the raw item
     */
    virtual void append(RawItem* item) = 0;

    /*
     * An interface for get raw item by index
     *
     *  @param[in]  index   the index of the raw item
     *  @return pointer to the raw item
     */
    virtual RawItem * at(int index) = 0;

    /*
     * An interface for get number of raw items
     *
     *  @return number of raw items.
     */
    virtual int size() = 0;
};

class RawCorpus {
public:
    RawCorpus() {}
    virtual ~RawCorpus() {}

    /*
     * An interface for append raw sentence to corpus.
     *
     *  @param[in]  sent    the pointer to the raw sentence
     */
    virtual void append(RawSentence* sent) = 0;

    /*
     * An interface for get raw sentence by index
     *
     *  @param[in]  index   the index of the raw sentence.
     *  @return pointer to the raw sentence.
     */
    virtual RawSentence * at(int index) = 0;

    /*
     * An interface for get number of raw sentence.
     *
     *  @return number of raw sentence.
     */
    virtual int size() = 0;
};

}

}

#endif  // end for __LTP_CORPUS_H__
