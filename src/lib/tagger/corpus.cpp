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
#include "corpus.h"

const char *
TagItem :: item() {
    return word.c_str();
}

const char *
TagItem :: tag() {
    return goldtag.c_str();
}

TagSent :: ~TagSent() {
    for (int i = 0; i < items.size(); ++ i) {
        delete items[i];
    }
}

void
TagSent :: append(RawItem *item) {
    items.push_back( item );
}

RawItem *
TagSent :: at(int index) {
    if (index < 0 || index > items.size()) {
        return NULL;
    } else {
        return items[index];
    }
}

int
TagSent :: size() {
    return items.size();
}

TagCorpus :: ~TagCorpus() {
    for (int i = 0; i < sents.size(); ++ i) {
        delete sents[i];
    }
}

void
TagCorpus :: append(RawSentence *sent) {
    sents.push_back( sent );
}

RawSentence *
TagCorpus :: at(int index) {
    if (index < 0 || index > sents.size()) {
        return NULL;
    } else {
        return sents[index];
    }
}

int
TagCorpus :: size() {
    return sents.size();
}
