/*
 *
 *
 *
 *
 *
 *
 *
 */
#ifndef __LTP_CONFIGURE_H__
#define __LTP_CONFIGURE_H__

#include <iostream>
#include <map>
#include "cppstrlib.h"

using namespace std;
using namespace ltp::utility;

class ltp_configure {
public:
    ltp_configure();
    ~ltp_configure();

    /*
     * Load config file
     *
     *  @param[in]  filename    name of config file.
     *  @return 0 if successfully loaded,
     *          -1 on failure.
     */
    int load_cfg(const char *filename);

    /*
     *
     *
     *
     *
     */
    void set_cfg(const char *key, const char *value);

    /*
     * Get configure item indicate by key
     *
     *  @param[in]  key The key.
     *  @return value if key exisit;
     *          empty str, otherwise
     */
    string config(const char *key);

    /*
     *
     *
     */
    void display();
private:
    map<string, string> items;
};

#endif  // end for __LTP_CONFIGURE_H__
