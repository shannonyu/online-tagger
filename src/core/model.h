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

    /*
     * Regist alphabet to the model.
     *
     *  @param[in]  name        the alphabet unique name
     *  @param[in]  alpha       the pointer to the alphabet
     *  @param[in]  overwrite   if the overwrite handle is true, the alphabet 
     *                          is overwrited when same-name alphabet is 
     *                          registed.
     */
    void registAlphabet(const char *name, Alphabet *alpha, bool overwrite = false);

    /*
     * Regist parameter to the model.
     *
     *  @param[in]  name        the parameter's unique name
     *  @param[in]  param       the pointer to the parameter
     *  @param[in]  overwrite   if the overwirte handle is true, the parameter
     *                          is overwrited when same-name alphabet is
     *                          registed.
     */
    void registParameter(const char *name, Parameter *param, bool overwrite = false);

    /*
     * Get alphabet of certain name.
     *
     *  @param[in]  name    the alphabet's name.
     *  @return     pointer to the alphabet.
     *              NULL is returned when error accurd.
     */
    Alphabet *getAlphabet(const char *name);

    /*
     * Get parameter of certain name.
     *
     *  @param[in]  name    the parameter's name.
     *  @return     pointer to the parameter.
     *              NULL is returned when error accured.
     */
    Parameter *getParameter(const char *name);

    /*
     *
     *
     *
     */
    int saveModel(const char *filename);

    /*
     *
     *
     *
     */
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
