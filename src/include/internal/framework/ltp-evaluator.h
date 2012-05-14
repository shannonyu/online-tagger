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
 */
#ifndef __LTP_EVALUATOR_H__
#define __LTP_EVALUATOR_H__

#include "ltp-data.h"

namespace ltp {

namespace framework {

/*
 * Class Evaluator
 *
 *  The evaluator class is for evaluate the accuracy,
 *  recall of the certain class.
 */

class Evaluator {
public:
    Evaluator() {}
    virtual ~Evaluator() {}

    /*
     * An interface to command start evaluation cummulation
     * When start is apply, the statistic is clear.
     */
    virtual void start() = 0;

    /*
     * An interface of command end evaluation cummulation
     * When end is apply evaluation will not be
     */
    virtual void end() = 0;

    /*
     * An interface of reporting the evaluate result.
     */
    virtual void report() = 0;

    /*
     * An interface of evaluating one instance.
     *
     *  @param[in]  result  the decode result
     *  @param[in]  inst    the gold instance
     *  @param[in]  accumulate  identify whether count this
     *              result into global results
     */
    virtual void evaluate(Labels *result, Instance *inst,
            bool accumulate = false) = 0;

    virtual double p() = 0;
    virtual double r() = 0;
    virtual double f() = 0;
};

}

}

#endif  // end for __LTP_EVALUATOR_H__
