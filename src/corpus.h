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
#ifndef __TagCorpus_H__
#define __TagCorpus_H__

#include <iostream>
#include <vector>

#include "ltp-corpus.h"

using namespace std;
using namespace ltp::framework;

class TagItem : public RawItem {
public:
    TagItem(string _word, string _tag) : word(_word), goldtag(_tag) {}
    ~TagItem() {}

    /*
     * Access the item.
     *
     *  @return the item.
     */
    const char *item();

    /*
     * Access the tag.
     *
     *  @return the tag.
     */
    const char *tag();
private:
    string word;
    string goldtag;
};

class TagSent : public RawSentence {
public:
    TagSent() {}
    ~TagSent();

    /*
     * Append a raw item to sentence.
     *
     *  @param[in]  item    the pointer to the item.
     */
    void append( RawItem *item );

    /*
     * Get the raw item by index
     *
     *  @param[in]  index   the index of the item.
     *  @return pointer to the item,
     *          NULL on failure.
     */
    RawItem * at(int index);

    /*
     * Get number of raw items.
     *
     *  @return number of items.
     */
    int size();
private:
    vector<RawItem *> items;
};

class TagCorpus : public RawCorpus {
public:
    TagCorpus() {}
    ~TagCorpus();

    /*
     * Append a raw sentence to corpus.
     *
     *  @param[in]  sent    the pointer to the sentence
     */
    void append( RawSentence *sent );

    /*
     * Get the raw sentence by index
     *
     *  @param[in]  index   the index of the sentence
     *  @return pointer to the item,
     *          NULL on failure.
     */
    RawSentence *at(int index);

    /*
     * Get number of raw sentences.
     *
     *  @return number of sentences.
     */
    int size();
private:
    vector<RawSentence *> sents;
};

#endif  // end for __TagCorpus_H__
