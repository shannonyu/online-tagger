/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
#include "cppstrlib.h"

using namespace ltp::utility;

string
cppstr :: chomp(string str) {
    int len = str.size();

	if (len == 0) {
		return "";
	}

    while (str[len - 1] == ' ' ||
            str[len - 1] == '\r' ||
            str[len - 1] == '\n' ) {
        str = str.substr(0, -- len);
    }
    while (str[0] == ' ' ||
            str[0] == '\r' ||
            str[0] == '\n') {
        str = str.substr(1, -- len);
    }
    return str;
}

string
cppstr :: cutoff(string str, string mark) {
    size_t pos = str.find(mark);

    if (pos == string::npos) {
        return str;
    } else {
        return str.substr(0, pos);
    }
}

vector<string>
cppstr :: split(string str) {
    vector<string> sep;

    while (str.size() > 0) {
        int i;
        for (i = 0; i < str.size(); ++ i) {
            if (str[i] == ' ' || str[i] == '\t') {
                break;
            }
        }
        sep.push_back( str.substr(0, i) );

        for (; i < str.size() && (str[i] == ' ' || str[i] == '\t'); ++ i);
        if (i == str.size()) {
           str = "";
        } else {
            str = str.substr(i);
        }
    }

    return sep;
}

vector<string>
cppstr :: split(string str, string separator) {
    vector<string> sep;

    while (str.size() > 0) {
        size_t pos = str.find(separator);
        sep.push_back(str.substr(0, pos));
        if (pos == string :: npos) {
            str = "";
        } else {
            str = str.substr(pos + separator.size());
        }
    }

    return sep;
}

string
cppstr :: join(vector<string> sep) {
    string ret = "";
    for (int i = 0; i < sep.size(); ++ i) {
        ret = ret + sep[i];
    }
    return ret;
}

string
cppstr :: join(vector<string>sep, string separator) {
    if (sep.size() == 0) return "";
    string ret = sep[0];
    for (int i = 1; i < sep.size(); ++ i) {
        ret = ret + separator + sep[i];
    }
    return ret;
}

bool
cppstr :: startswith(const string &str, const string &head) {
    int len = head.size();

    return (str.substr(0, len) == head);
}

bool
cppstr :: isInt(const string &str) {
    int i = 0;
    if (str[0] == '-')
        i = 1;

    for (; i < str.size(); ++ i) {
        if (false == (str[i] >= '0' && str[i] <= '9')) {
            return false;
        }
    }
    return true;
}

bool
cppstr :: isDouble(const string &str) {
    int i = 0;
    int state = 0;
    if (str[0] == '-')
        i = 1;

    for (; i < str.size(); ++ i) {
        if (str[i] == '.') {
            ++ state;
            if (state > 1) return false;
        } else if (false == (str[i] >= '0' && str[i] <= '9')) {
            return false;
        }

    }
    return true;
}

int
cppstr :: toInt(const string &str) {
    int ret = 0;
    int sign = 1;
    int i = 0;
    if ('-' == str[0]) {
        sign = -1;
        i = 1;
    }
    for (; i < str.size(); ++ i) {
        ret *= 10;
        ret += str[i] - '0';
    }
    return sign * ret;
}

double
cppstr :: toDouble(const string &str) {
    double x = 0.0, y = 1.0;
    double sign = 1.0;
    int i = 0;
    if ('-' == str[0]) {
        sign = -1.0;
        i = 1;
    }
    for (; i < str.size() && str[i] != '.'; ++ i) {
        x *= 10.0;
        x += (str[i] - '0');
    }

    for (++ i; i < str.size(); ++ i) {
        y /= 10.0;
        x += (str[i] - '0') * y;
    }

    return x * sign;

}
