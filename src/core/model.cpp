/* COPYRIGHT INFORMATION
 *
 *
 *
 *
 *
 *
 *
 *
 */
#include <cstring> // for strcmp

#include "model.h"
#include "cppstrlib.h"
using namespace ltp::utility;


Model::Model() {
    m_NumAlpha = 0;
    m_CapAlpha = 0;

    m_NumParam = 0;
    m_CapParam = 0;

    m_AlphaName = NULL;
    m_ParamName = NULL;
    m_Alpha     = NULL;
    m_Param     = NULL;
}

Model::~Model() {
    for (int i = 0; i < m_NumAlpha; ++ i) {
        delete m_AlphaName[i];
    }
    delete m_AlphaName;

    for (int i = 0; i < m_NumParam; ++ i) {
        delete m_ParamName[i];
    }

    delete m_ParamName;
    delete m_Alpha;
    delete m_Param;
}

void Model::registAlphabet(const char *name, Alphabet *alpha, bool overwrite) {
    for (int i = 0; i < m_NumAlpha; ++ i) {
       if (strcmp(m_AlphaName[i], name) == 0) {
           if (overwrite) {
               m_Alpha[i] = alpha;
           }
           return;
       }
    }

    if (m_NumAlpha >= m_CapAlpha) {
        m_CapAlpha = ((m_CapAlpha << 1) + 1);
        m_AlphaName = (char **)realloc(m_AlphaName, m_CapAlpha * sizeof(char *));
        m_Alpha = (Alphabet **)realloc(m_Alpha, m_CapAlpha * sizeof(Alphabet *));
    }

    int len = strlen(name);
    m_AlphaName[m_NumAlpha] = (char *)calloc(len + 1, sizeof(char));
    strncpy(m_AlphaName[m_NumAlpha], name, len + 1);
    m_Alpha[m_NumAlpha] = alpha;
    m_NumAlpha ++;
}

void
Model::registParameter(const char *name, Parameter *param, bool overwrite) {
    for (int i = 0; i < m_NumParam; ++ i) {
        if (strcmp(m_ParamName[i], name) == 0) {
            if (overwrite) {
                m_Param[i] = param;
            }
            return;
        }
    }

    if (m_NumParam >= m_CapParam) {
        m_CapParam = ((m_CapParam << 1) + 1);
        m_ParamName = (char **)realloc(m_ParamName, m_CapParam * sizeof(char *));
        m_Param = (Parameter **)realloc(m_Param, m_CapParam * sizeof(Parameter *));
    }

    int len = strlen(name);
    m_ParamName[m_NumParam] = (char *)calloc(len + 1, sizeof(char));
    strncpy(m_ParamName[m_NumParam], name, len + 1);
    m_Param[m_NumParam] = param;
    m_NumParam ++;
}

Alphabet *
Model::getAlphabet(const char *name) {
    for (int i = 0; i < m_NumAlpha; ++ i) {
        if (strcmp(m_AlphaName[i], name) == 0) {
            return m_Alpha[i];
        }
    }
    return NULL;
}

Parameter *
Model::getParameter(const char *name) {
    for (int i = 0; i < m_NumParam; ++ i) {
        if (strcmp(m_ParamName[i], name) == 0) {
            return m_Param[i];
        }
    }
    return NULL;
}

int Model::saveModel(const char *filename) {
    FILE *fp = fopen(filename, "w");

    if (NULL == fp) {
        return -1;
    }

    for (int i = 0; i < m_NumAlpha; ++ i) {
        fprintf(fp, "#%s#\n", m_AlphaName[i]);
        m_Alpha[i]->save(fp);
        fprintf(fp, "#END#\n");
    }

    for (int i = 0; i < m_NumParam; ++ i) {
        fprintf(fp, "#%s#\n", m_ParamName[i]);
        m_Param[i]->save(fp);
        fprintf(fp, "#END#\n");
    }

    return 0;
}

int Model::loadModel(const char *filename) {
    FILE *fp = fopen(filename, "r");

    if (NULL == fp) {
        return -1;
    }

    char buff[2048];
    string line;

    int state = 0;
    while (state != 7) {
        if (fgets(buff, 2048, fp) == 0) {
            break;
        }

        line = cppstr::chomp(string(buff));
        if (line.size() == 0) continue;

        if (line[0] == '#') {
            string name = line.substr(1, line.length() - 2);

            int i;
            for (i = 0; i < m_NumAlpha; ++ i) {
                if (strcmp(name.c_str(), m_AlphaName[i]) == 0) {
                    break;
                }
            }

            if (i < m_NumAlpha) {
                if (m_Alpha[i]->load(fp) != 0) {
                    fprintf(stderr, "ERROR: load alphbet [%s] failed.\n", m_AlphaName[i]);
                    return -1;
                }
                continue;
            }

            for (i = 0; i < m_NumParam; ++ i) {
                if (strcmp(name.c_str(), m_ParamName[i]) == 0) {
                    break;
                }
            }

            if (i < m_NumParam) {
                if (m_Param[i]->load(fp) != 0) {
                    fprintf(stderr, "ERROR: load parameter [%s] failed.\n", m_AlphaName[i]);
                    return -1;
                }
            } else {
                fprintf(stderr, "ERROR: %s is not registed\n", name.c_str());
                while (line != "#END#") {
                    fgets(buff, 2048, fp);
                    line = cppstr::chomp(string(buff));
                }
            }
        }
    }
    return 0;
}
