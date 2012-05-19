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
#ifndef __TAG_MODEL_H__
#define __TAG_MODEL_H__

#include "ltp-alphabet.h"
#include "ltp-parameter.h"

using namespace ltp::framework;

/*
 * Class TagModel
 *
 *  The TagModel class load model from file and
 *  save model from file.
 *
 *  It's a part of the frontend code.
 */
class Model {
public:
    Model();
    ~Model();

    void registAlphabet(const char *name, Alphabet *alpha, bool overwrite = false);
    void registParameter(const char *name, Parameter *param, bool overwrite = false);
    int numAlphabet(const char *name);
    int numParameter(const char *name);
    int saveModel(const char *filename);
    int loadModel(const char *filename);

private:
    int m_NumAlpha;
    int m_CapAlpha;
    int m_NumParam;
    int m_CapParam;

    char **m_AlphaName;
    char **m_ParamName;
    Alphabet  **m_Alpha;
    Parameter **m_Param;
};

#endif  // end for __TAG_MODEL_H__
