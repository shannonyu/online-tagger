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
#include "seg-reader.h"

#include "corpus.h"
#include "cppstrlib.h"

#define __GBK__ 0

#if __GBK__
#include "gbk.h"
#else
#include "utf.h"
#endif

using namespace ltp::utility;

SegmentReader :: SegmentReader() {
}

SegmentReader :: ~SegmentReader() {
}

int
SegmentReader :: open(const char *filename) {
    fp = fopen(filename, "r");
    if (NULL == fp) return -1;
    return 0;
}

bool
SegmentReader :: hasNext() {
    return (feof(fp) == false);
}

RawSentence *
SegmentReader :: getNext() {
    char buff[2048];

    if (fgets(buff, 2048, fp) == NULL) {
        return NULL;
    }

    RawSentence *sent = new TagSent();
    string line = cppstr::chomp(string(buff));

    if (line.size() == 0) {
        return NULL;
    }

    vector<string> words = cppstr::split(line);

    for (int i = 0; i < words.size(); ++ i) {
        vector<string> chars;
#if __GBK__
        int numChars = GBK::getCharactersFromGBKString(words[i], &chars);
#else
        int numChars = UTF::getCharactersFromUTF8String(words[i], &chars);
#endif
        if (1 == numChars) {
            string item = chars[0];
            string tag = "S";
            sent->append(new TagItem(item, tag));
        } else {
            for (int j = 0; j < numChars; ++ j) {
                string item = chars[j];
                string tag = "";
                if (j == 0) {
                    tag = "B";
                } else if (j == numChars - 1) {
                    tag = "E";
                } else {
                    tag = "M";
                }

                sent->append(new TagItem(item, tag));
            }
        }
    }

    return sent;
}

RawCorpus *
SegmentReader :: read(const char *filename) {
    if (-1 == this->open(filename)) {
        return NULL;
    }

    RawCorpus *corpus = new TagCorpus();
    while (true == hasNext()) {
        RawSentence *sent = getNext();
        if (NULL != sent) {
            corpus->append(sent);
        }
    }
    return corpus;
}
