/*
 *
 *
 *
 *
 *
 *
 *
 */
#include "ltp-configure.h"

ltp_configure::ltp_configure() {
}

ltp_configure::~ltp_configure() {
}

int
ltp_configure::load_cfg(const char *filename) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        return -1;
    }
    char buff[1024];

    while (fgets( buff, 1024, fp ) ) {
        string line = buff;

        line = cppstr::chomp(line);
        line = cppstr::cutoff(line, "#");
        if (line.size() == 0) {
            continue;
        }

        vector<string> sep = cppstr::split(line, "=");

        if (sep.size() != 2) {
            return -1;
        }

        sep[0] = cppstr::chomp(sep[0]);
        sep[1] = cppstr::chomp(sep[1]);

        items[ sep[0] ] = sep[1];
    }

    return 0;
}

void
ltp_configure::set_cfg(const char *key, const char *value) {
    items[string(key)] = string(value);
}

string
ltp_configure::config(const char *key) {
    if (items.find(string(key)) != items.end()) {
        return items[ string(key) ];
    } else {
        return "";
    }
}

void
ltp_configure::display() {
    fprintf(stderr, "config items include:\n");
    for (map<string, string> :: iterator itx = items.begin();
            itx != items.end(); itx ++) {
        fprintf(stderr, "      %-10s %-10s\n", itx->first.c_str(),
                itx->second.c_str());
    }
    fprintf(stderr, "\n");
}
