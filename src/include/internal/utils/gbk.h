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
#ifndef __UTILITY_GBK_H__
#define __UTILITY_GBK_H__

#include <cassert>
#include <iostream>
#include <string>

namespace GBK {

/*==============================================================
 *
 * GBK string and character utils
 *
 *==============================================================*/

/*--------------------------------------------------------------
 *
 *  getGBKStringLength - get how many characters are in a GBK string
 *
 *--------------------------------------------------------------*/

inline
unsigned long int getGBKStringLength(const std::string &s) {
    unsigned long int retval = 0;
    unsigned long int idx = 0;

    while (idx<s.length()) {
        if ((s[idx]&0x80) == 0) {
            ++ idx;
            ++ retval;
        } else {
            idx += 2;
            ++ retval;
        }
    }

    return retval;
}

/*--------------------------------------------------------------
 *
 * getCharactersFromGBString - get the characters from gbk 
 *                             string. The characters from this
 *                             string are appended to a given
 *                             sentence.
 *
 *--------------------------------------------------------------*/
template<class CSentence>
inline int getCharactersFromGBKString(const std::string &s, CSentence *sentence) {
    if (sentence == NULL)
        return 0;

    unsigned long int idx=0;
    unsigned long int len=0;

    while (idx<s.length()) {
        if ((s[idx]&0x80)==0) {
            sentence->push_back(s.substr(idx, 1));
            ++ len;
            ++ idx;
        } else {
            sentence->push_back(s.substr(idx, 2));
            ++ len;
            idx += 2;
        }
    }

    return len;
}

/*--------------------------------------------------------------
 *
 * getFirstCharFromString - get the frist character from gbk
 *                          string.
 *
 *--------------------------------------------------------------*/
inline string getFirstCharFromGBKString(const std::string &s) {
    if (s=="")
        return "";
    if ((s[0]&0x80) == 0) {
        return s.substr(0, 1);
    } else {
        return s.substr(0, 2);
    }
}

/*--------------------------------------------------------------
 *
 * getLastCharFromString - get the last character from gbk
 *                         string.
 *
 *--------------------------------------------------------------*/
inline string getLastCharFromGBKString(const std::string &s) {
    if (s=="")
        return "";
    unsigned long int idx=0;
    std::string retval;

    while (idx<s.length()) {
        if ((s[idx]&0x80)==0) {
            retval = s.substr(0, 1);
            ++ idx;
        } else {
            retval = s.substr(0, 2);
            idx += 2;
        }
    }

    return retval;
}

}

#endif
