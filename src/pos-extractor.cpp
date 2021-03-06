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
#include "pos-extractor.h"
#include "c-data.h"
#include "def.h"

#if __GBK__
    #include "gbk.h"
#else
    #include "utf.h"
#endif

#include <sstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace ltp::framework;

#define __FEATRUE_PLUS__ 0

PostagExtractor :: PostagExtractor(Alphabet* featureDict,
        Alphabet* labelDict,
        Alphabet* wordDict) : m_FeatureDict(featureDict), 
    m_LabelDict(labelDict), 
    m_WordDict(wordDict){
}

PostagExtractor :: ~PostagExtractor() {
}


/*
 * Extract training/testing data from raw corpus.
 *
 *  @param  corpus[RawCorpus *] the training/testing data
 *  @param  append[bool]        specify whether append the feature string
 *                              to the alphabet
 */
Data *
PostagExtractor :: extract(RawCorpus *corpus, bool append) {
    Data *data = new CData();

    if (append) {
        for (int i = 0; i < corpus->size(); ++ i) {
            RawSentence *sent = corpus->at(i);
            for (int j = 0; j < sent->size(); ++ j) {
                m_LabelDict->insert(sent->at(j)->tag());
                m_WordDict->insert(sent->at(j)->item());
            }
        }
    }

    for (int i = 0; i < corpus->size(); ++ i) {
        data->append(extract(corpus->at(i), append));
        // fprintf(stderr, "%d\r", i+1);
    }
    return data;
}

/*
 * Function for extract feature vector from RawSentence
 *
 *  @param  sent[RawSentence *] pointer to the raw sentence
 *  @param  append[bool]        specify whether append feature string
 *                              to alphabet
 *  @return Instance *          pointer to the instance
 */
Instance *
PostagExtractor :: extract(RawSentence *sent, bool append) {
    Items *items = new CItems();
    Labels *labels = new CLabels();

    int len = sent->size();
    int numLabels = m_LabelDict->size();

    for (int i = 0; i < len; ++ i) {
        Item *item = new CItem(numLabels, m_WordDict->insert(sent->at(i)->item()));

        vector<string> featStrCache;
        vector<int>    featIntCache;
        vector<string> chars;

        string curr_word  = sent->at(i)->item();
        string prev_word  = (i - 1 >= 0)  ? sent->at(i - 1)->item() : "__BOS__";
        string prev_2word = (i - 2 >= 0)  ? sent->at(i - 2)->item() : "__BOS__";
        string next_word  = (i + 1 < len) ? sent->at(i + 1)->item() : "__EOS__";
        string next_2word = (i + 2 < len) ? sent->at(i + 2)->item() : "__EOS__";
#if __GBK__
        string prev_char  = (i - 1 >= 0)  ? GBK::getLastCharFromGBKString(prev_word)  : "__BOC__";
        string next_char  = (i + 1 < len) ? GBK::getFirstCharFromGBKString(next_word) : "__EOC__";
        int num_chars = GBK::getCharactersFromGBKString< vector<string> >(curr_word, &chars);
#else
        string prev_char  = (i - 1 >= 0)  ? UTF::getLastCharFromUTF8String(prev_word)  : "__BOC__";
        string next_char  = (i + 1 < len) ? UTF::getFirstCharFromUTF8String(next_word) : "__EOC__";
        int num_chars = UTF::getCharactersFromUTF8String< vector<string> >(curr_word, &chars);
#endif

        featStrCache.push_back("UWT[-2,0]="  + prev_2word);
        featStrCache.push_back("UWT[-1,0]="  + prev_word);
        featStrCache.push_back("UWT[0,0]="   + curr_word);
        featStrCache.push_back("UWT[1,0]="   + next_word);
        featStrCache.push_back("UWT[2,0]="   + next_2word);
        featStrCache.push_back("BWT[-1,0]="  + prev_word + "/" + curr_word);
        featStrCache.push_back("BWT[0,0]="   + curr_word + "/" + next_word);
        featStrCache.push_back("B2WT[-1,0]=" + prev_word + "/" + next_word);
        featStrCache.push_back("CW[-1,0]="   + prev_char + "/" + curr_word);
        featStrCache.push_back("CW[1,0]="    + next_char + "/" + curr_word);

        ostringstream lengthout; lengthout << num_chars;
        featStrCache.push_back("Length=" + lengthout.str());

        // Handle exception
        // There is case where `_np` leading to no form but tag
        if (num_chars)
            featStrCache.push_back("BE=" + chars[0] + "/" + chars[num_chars - 1]);
        else {
            fprintf(stderr, "===>> bug detected <<===\n");
            fprintf(stderr, "sent:[");
            for(int x = 0; x < len; ++ x) {
                fprintf(stderr, "%s", sent->at(x)->item());
                if (x < len - 1) fprintf(stderr, "|");
            }
            fprintf(stderr, "]\n");
            fprintf(stderr, "i:[%d]\n", i);
        }

        string prefix = "";
        for (int j = 0; j < 3 && j < num_chars; ++ j) {
            prefix = prefix + chars[j];              featStrCache.push_back("Prefix=" + prefix);
        }

        string suffix = "";
        for (int j = num_chars - 1; j >= 0 && j >= num_chars - 3; -- j) {
            suffix = chars[j] + suffix;              featStrCache.push_back("Suffix=" + suffix);
        }

        for(int j = 0; j < chars.size(); j++) {
            prefix = chars[0] + chars[j];            featStrCache.push_back("Pre2fix=" + prefix);
            suffix = chars[num_chars- 1] + chars[j]; featStrCache.push_back("Suf2fix=" + suffix);
            featStrCache.push_back("CurChar" + chars[j]);

            if(j +1 < chars.size() && chars[j] == chars[j+1]) {
                featStrCache.push_back("dulChar=" + chars[j]);
            }

            if(j +2 < chars.size() && chars[j] == chars[j+2]) {
                featStrCache.push_back("dul2Char=" + chars[j]);
            }
        }

#if __FEATURE_PLUS__
        int sz = featStrCache.size();
        for (int j = 0; j < sz; ++ j) {
            featStrCache.push_back( featStrCache[j] + "/" + curr_word );
        }
#endif  // end for __FEATURE_PLUS__

        featIntCache.resize(featStrCache.size());

        for (int j = 0; j < featStrCache.size(); ++ j) {
            if (append) {
                featIntCache[j] = m_FeatureDict->insert(featStrCache[j]);
            } else {
                featIntCache[j] = m_FeatureDict->lookup(featStrCache[j]);
            }
        }

        for (int k = 0; k < featIntCache.size(); ++ k) {
            item->append(featIntCache[k], 0);
        }

        items->append(item);
        labels->append(m_LabelDict->lookup(sent->at(i)->tag()));
    }

    return new CInstance( items, labels );
}
