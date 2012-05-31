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
#include "pos-reader.h"
#include "cppstrlib.h"
#include "corpus.h"

using namespace ltp::utility;

PostagReader::PostagReader() {
}

PostagReader::~PostagReader() {
}

int
PostagReader::open( const char *filename ) {
    fp = fopen(filename, "r");
    if (fp == NULL) return -1;
    else return 0;
}

bool
PostagReader::hasNext() {
    return (feof(fp) == false);
}

RawSentence *
PostagReader::getNext() {
    char buff[10005];

    if (fgets(buff, 10005, fp) == NULL) {
        return NULL;
    }

    RawSentence *sent = new TagSent();
    string line = cppstr::chomp(string(buff));

    if (line.size() == 0) {
        return NULL;
    }

    vector<string> words = cppstr::split(line);

    for (int i = 0; i < words.size(); ++ i) {
        vector<string> sep = cppstr::split(words[i], "_");
        if (sep.size() > 2) {
            string tag = sep[sep.size() - 1];
            sep.pop_back();
            string word = cppstr::join(sep, "_");

            sent->append(new TagItem(word, tag));

        } else if (sep.size() == 2) {
            string word = sep[0];
            string tag  = sep[1];

            sent->append(new TagItem(word, tag));
        } else {
            return NULL;
        }
    }

    return sent;
}

RawCorpus *
PostagReader::read(const char *filename) {
    if(-1 == open(filename)) {
        return NULL;
    }

    RawCorpus *corpus = new TagCorpus();
    while (hasNext()) {
        RawSentence *sent = getNext();
        if (NULL != sent) {
            corpus->append(sent);
        }
    }

    return corpus;
}
