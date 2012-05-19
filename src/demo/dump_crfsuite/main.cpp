#include <iostream>
#include <cstdio>
#include "corpus.h"
#include "alphabet.h"
#include "data.h"
#include "segment-reader.h"
#include "segment-extractor.h"
#include "postag-reader.h"
#include "postag-extractor.h"
#include "ltp-argtable.h"

using namespace std;

int main(int argc, const char **argv) {

    ltp_argtable *argtable = new ltp_argtable();
    argtable->add_option("t", "type", "str", "type", "identify the convert type");
    argtable->add_option("f", "file", "str", "file", "identify the input file path");
    argtable->add_option("d", "dict", "str", "dict", "identify the dict path");
    if (argtable->parse_args(-- argc, ++ argv) == -1) {
        argtable->print_glossary();
        return -1;
    }

    string file = "";
    string type = "";
    string dict = "";

    if (NULL != *(const char **)argtable->option("file"))
        file = *(const char **)argtable->option("file");
    if (NULL != *(const char **)argtable->option("type"))
        type = *(const char **)argtable->option("type");
    if (NULL != *(const char **)argtable->option("dict"))
        dict = *(const char **)argtable->option("dict");

    fprintf(stderr, "[%s] | [%s] | [%s]\n", file.c_str(), type.c_str(), dict.c_str());

    Reader *reader = NULL;
    if (type == "postag") {
        reader = new PostagReader();
    } else if (type == "segment") {
        reader = new SegmentReader();
    } else {
        fprintf(stderr, "Unknown type.\n");
        return -1;
    }

    RawCorpus *corpus = reader->read(file.c_str());

    fprintf(stderr, "read corpus is done.\n");

    Alphabet *featureDict = new HashDict();
    Alphabet *labelDict   = new HashDict();
    Alphabet *wordDict    = new HashDict();

    Extractor *extractor = NULL;
    if (type == "postag") {
        extractor = new PostagExtractor(featureDict, labelDict, wordDict);
    } else if (type == "segment") {
        extractor = new SegmentExtractor(dict.c_str(),
                featureDict, labelDict, wordDict);
    } else {
        fprintf(stderr, "Unknown type.\n");
        return -1;
    }

    Data *data = extractor->extract(corpus, true);

    fprintf(stderr, "extract is done.\n");

    for (int i = 0; i < data->size(); ++ i) {
        Instance *inst = data->at(i);
        Items *items = inst->items();
        Labels *labels = inst->labels();

        for  (int j = 0; j < items->size(); ++ j) {
            printf("%s", labelDict->rlookup(labels->at(j)));

            Item *item = items->at(j);
            for (int k = 0; k < item->size(); ++ k) {
                printf("\t%s", featureDict->rlookup(item->at(k)));
            }
            printf("\n");
        }
        printf("\n");
    }
    return 0;
}
