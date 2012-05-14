/*
 *
 *
 *
 *
 *
 */
#ifndef __LTP_ALPHABET_H__
#define __LTP_ALPHABET_H__

#include <iostream>

namespace ltp {

namespace framework {

/*
 * Class Alphabet
 *
 *  The alphabet class implement the function of
 *  converting feature string to integer and
 *  store.
 *
 *  The alphabet class plays in the frontend.
 */
class Alphabet {
public:
    Alphabet() {}
    virtual ~Alphabet() {}

    /*
     * A wrapper for looking up the index of
     * certain string in the alphabet.
     *
     *  @param[in]  str,    the string
     *  @return     int     the index
     */
    virtual int lookup(const std::string &str) = 0;

    /*
     * A wrapper for insert a certain string
     * to the alphabet.
     *
     *  @param[in]  str,    the string
     *  @return     int     the index
     */
    virtual int insert(const std::string &str) = 0;

    /*
     * An interface for looking up the index of
     * certain string in the alphabet.
     *
     *  @param[in]  str,    the string
     *  @return     int     the index
     */
    virtual int lookup(const char *str) = 0;

    /*
     * An interface for look up the string value
     * of certain rank
     *
     *  @param[in]  rank    the rank
     *  @return     const char * the string
     */
    virtual const char *rlookup(int rank) = 0;

    /*
     * An interface for insert a certain string
     * to the alphabet.
     *
     *  @param[in]  str,    the string
     *  @return     int     the index
     */
    virtual int insert(const char *str) = 0;

    /*
     * An interface for load alphabet from file
     *
     *  @param[in]  fp,     the file stream
     *  @return     int     0, on success
     *                      -1, on failed.
     */
    virtual int load(FILE *fp) = 0;

    /*
     * An interface for save alphabet to file
     *
     *  @param[in]  fp,     the file stream
     *  @return     int     0, on success
     *                      -1, on failed.
     */
    virtual int save(FILE *fp) = 0;

    /*
     * An interface for obtain size number of
     * element in the alphabet.
     *
     *  @return     int     number of element
     */
    virtual int size() = 0;
};

}

}

#endif  // end for __LTP_ALPHABET_H__
