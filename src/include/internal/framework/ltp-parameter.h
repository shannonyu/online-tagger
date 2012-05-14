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
#ifndef __LTP_PARAMETER_H__
#define __LTP_PARAMETER_H__

#include <iostream>

namespace ltp {

namespace framework {

/*
 * Class Paramter
 *
 *  The paramter class provide interface for accessing the
 *  parameter storage with update, value, averaged method
 *  and so on.
 */
class Parameter {
public:
    Parameter() {}
    virtual ~Parameter() {}

    /*
     * An interface of averaging parameter.
     *
     *  @param[in]  value   the average value.
     */
    virtual void averaged(double value) = 0;

    /*
     * An interface of updating parameter without iteration
     * time information.
     *
     *  @param[in]  key     the key to be updated,
     *  @param[in]  value   the value to be updated.
     */
    virtual void update(int key, double value) = 0;

    /*
     * An interface of updating parameter with iteration time
     * information
     *
     *  @param[in]  key     the key to be updated,
     *  @param[in]  value   the value to be updated,
     *  @param[in]  curUpdSeq   the iteration time.
     */
    virtual void update(int key, double value, double curUpdSeq) = 0;

    /*
     * An interface of getting value of certain key.
     *
     *  @param[in]  key     the key.
     *  @return     double  the value of the key.
     */
    virtual double value(int key) = 0;

    /*
     * An interface of saving parameter to file stream
     *
     *  @param[in]  fp      the file stream.
     *  @return     int     0, on success,
     *                      -1, otherwise.
     */
    virtual int save(FILE *fp) = 0;

    /*
     * An interface of loading parameter from file stream
     *
     *  @param[in]  fp      the file stream.
     *  @return     int     0, on success.
     *                      -1, otherwise.
     */
    virtual int load(FILE *fp) = 0;

    /*
     * An interface of obtaining the size of parameters.
     *
     *  @return     int     the size of paramters.
     */
    virtual int size() = 0;
};

}

}

#endif  // end for __LTP_PARAMETER_H__
