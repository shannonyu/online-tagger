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
#ifndef __OT_WORDSEG_DLL_H__
#define __OT_WORDSEG_DLL_H__

#include <string>
#include <vector>

typedef void * otws_t;

// load config file
otws_t OTWS_Load(const char *cfg_file);

// destroy otws_engine
int OTWS_Destroy(otws_t handle);

// wordseg
int OTWS_Wordseg_x(otws_t handle, 
        const std::string& sent, 
        std::vector<std::string>& words);
// int OTWS_Wordseg(otws_t *engine, const char *sent, char **words);

#endif  // end for __OT_WORDSEG_DLL_H__
