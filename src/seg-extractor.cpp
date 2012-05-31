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
#include "seg-extractor.h"

#include "c-data.h"
#include "cppstrlib.h"
#include <sstream>

#include "ltp-log.h"

using namespace ltp::utility;


SegmentExtractor::SegmentExtractor(
        const char *filename,
        Alphabet *featureDict,
        Alphabet *labelDict,
        Alphabet *charDict) : m_FeatureDict(featureDict),
    m_LabelDict(labelDict), 
    m_CharDict(charDict) {

    char buff[2048];

    FILE *fp = fopen(filename, "r");
    if (NULL != fp) {
        while (fgets(buff, 2048, fp)) {
            string line = cppstr::chomp(string(buff));
            vector<string> sep = cppstr::split(line);

            if (2 == sep.size() && "word" == sep[0]) {
                m_Lexicon.insert(sep[0]);
            }
        }
    }
    fclose(fp);
}

SegmentExtractor::~SegmentExtractor() {
}

Data*
SegmentExtractor::extract(RawCorpus *corpus, bool append) {
    Data *data = new CData();

    if (append) {
        for (int i = 0; i < corpus->size(); ++ i) {
            RawSentence *sent = corpus->at(i);
            for (int j = 0; j < sent->size(); ++ j) {
                m_LabelDict->insert(sent->at(j)->tag());
                m_CharDict->insert(sent->at(j)->item());
            }
        }
    }

    for (int i = 0; i < corpus->size(); ++ i) {
        data->append(extract(corpus->at(i), append));
    }

    return data;
}

Instance* 
SegmentExtractor::extract(RawSentence *sent, bool append) {
    Items *items = new CItems();
    Labels *labels = new CLabels();

    int numLabels = m_LabelDict->size();

    int len = sent->size();
    vector< vector<string> > subSentence(len + 1, vector<string>(len + 1, ""));
    vector< vector<int> > lexiconCache(len, vector<int>(3, -1));

    int maxWordLengthFeature = 5;
    for (int i = 0; i < len; ++ i) {
        subSentence[i][i] = "";
        for (int j = i; j < len && j < i + 5; ++ j) {
            subSentence[i][j + 1] = subSentence[i][j] + sent->at(j)->item();
            if (m_Lexicon.find(subSentence[i][j + 1]) != m_Lexicon.end()) {
                int l = j + 1 - i;

                if (l > lexiconCache[i][0])
                    lexiconCache[i][0] = l;

                if (l > lexiconCache[i][1])
                    lexiconCache[i][1] = l;

                for (int k = i + 1; k < j; ++ k)
                    if (l > lexiconCache[i][2])
                        lexiconCache[i][2] = l;
            }
        }
    }

    for (int i = 0; i < len; ++ i) {
        // Item *item = new CItem(m_CharDict->insert(sent->at(i)->item()));
        Item *item = new CItem(m_LabelDict->size(),
                m_CharDict->insert(sent->at(i)->item()));

        vector<string> featStrCache;
        vector<int>    featIntCache;

        string currChar  = sent->at(i)->item();
        string prevChar  = (i - 1 >= 0) ? sent->at(i - 1)->item() : "__BOC__";
        string prev2Char = (i - 2 >= 0) ? sent->at(i - 2)->item() : "__BOC__";
        string nextChar  = (i + 1 < len) ? sent->at(i + 1)->item() : "__EOC__";
        string next2Char = (i + 2 < len) ? sent->at(i + 2)->item() : "__EOC__";

        featStrCache.push_back("UCT[-2,0]=" + prev2Char);
        featStrCache.push_back("UCT[-1,0]=" + prevChar);
        featStrCache.push_back("UCT[0,0]="  + currChar);
        featStrCache.push_back("UCT[1,0]="  + nextChar);
        featStrCache.push_back("UCT[2,0]="  + next2Char);

        featStrCache.push_back("BCT[-2,0]=" + prev2Char + "/" + prevChar);
        featStrCache.push_back("BCT[-1,0]=" + prevChar  + "/" + currChar);
        featStrCache.push_back("BCT[0,0]="  + currChar  + "/" + nextChar);
        featStrCache.push_back("BCT[1,0]="  + nextChar  + "/" + next2Char);

        featStrCache.push_back("B2CT[-2,0]=" + prev2Char + "/" + currChar);
        featStrCache.push_back("B2CT[-1,0]=" + prevChar  + "/" + nextChar);
        featStrCache.push_back("B2CT[0,0]="  + currChar  + "/" + next2Char);

        featStrCache.push_back("TCT[-1,0]" + prevChar + "/" + currChar + "/" + nextChar);

        //featStrCache.push_back(
        if (currChar == prev2Char) {
            featStrCache.push_back("-2ABABT");
        }
        if (prevChar == nextChar) {
            featStrCache.push_back("-1ABABT");
        }
        if (currChar == next2Char) {
            featStrCache.push_back("0ABABT");
        }
        if (currChar == prevChar) {
            featStrCache.push_back("-1AABBT");
        }
        if (currChar == nextChar) {
            featStrCache.push_back("0AABBT");
        }

        if (lexiconCache[i][0] > 0) {
            ostringstream S; S << lexiconCache[i][0];
            featStrCache.push_back("startdict=" + S.str());
        }

        if (lexiconCache[i][1] > 0) {
            ostringstream S; S << lexiconCache[i][1];
            featStrCache.push_back("enddict=" + S.str());
        }

        if (lexiconCache[i][2] > 0) {
            ostringstream S; S << lexiconCache[i][2];
            featStrCache.push_back("middict=" + S.str());
        }

        featIntCache.resize(featStrCache.size());

        for (int j = 0; j < featStrCache.size(); ++ j) {
            if (append) {
                featIntCache[j] = m_FeatureDict->insert(featStrCache[j]);
            } else {
                featIntCache[j] = m_FeatureDict->lookup(featStrCache[j]);
            }
        }

        for (int k = 0; k < featIntCache.size(); ++ k) {
            if (featIntCache[k] >= 0)
                item->append(featIntCache[k], 0);
        }

        items->append(item);
        labels->append(m_LabelDict->lookup(sent->at(i)->tag()));
    }

    return new CInstance(items, labels);
}
