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
#ifndef __LTP_MODEL_H__
#define __LTP_MODEL_H__

namespace ltp {

namespace framework {

/*
 * Class Model
 *
 *  The model class provide interface for saving model to file
 *  and loading model from file.
 */
class Model {
public:
    Model() {}
    virtual ~Model() {}

    /*
     * interface for registing alphabet to the model
     *
     *  @param  name    the name of the alphabet
     *  @param  alpha   the alphabet instance
     */
    void registAlphabet(const char *name, Alphabet *alpha) = 0;

    /*
     * interface for registing parameter to the model
     *
     *  @param  name    the name of the parameter
     *  @param  param   the parameter instance
     */
    void registParameter(const char *name, Parameter *param) = 0;

    /*
     * An interface for saving model.
     *
     *  @param[in]  filename    the file name.
     *  @return     int         0, on success,
     *                          -1, otherwise.
     */
    virtual int saveModel(const char *filename) = 0;

    /*
     * An interface for loading model.
     *
     *  @param[in]  filename    the file name.
     *  @return     int         0, on success,
     *                          -1, otherwise.
     */
    virtual int loadModel(const char *filename) = 0;
}

};

}

#endif  // end for __LTP_MODEL_H__
