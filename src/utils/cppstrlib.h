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
#ifndef __CPPSTRLIB_H__
#define __CPPSTRLIB_H__

#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace ltp {

namespace utility {

namespace cppstr {

string chomp(string str);
string cutoff(string str, string mark);
vector<string> split(string str);
vector<string> split(string str, string seperator);
string join(vector<string> sep);
string join(vector<string> sep, string separator);
bool startswith(const string &str, const string &head);
bool isInt(const string &str);
bool isDouble(const string &str);
int toInt(const string &str);
double toDouble(const string &str);
}

}

}

#endif  // end for __CPPSTRLIB_H__
