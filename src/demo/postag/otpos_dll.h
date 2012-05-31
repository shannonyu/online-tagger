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
#ifndef __OTPOS_DLL_H__
#define __OTPOS_DLL_H__

#include <string>
#include <vector>

typedef void * otpos_t;

// load config file
otpos_t OTPOS_Load(const char *cfg);

// destroy the engine
int OTPOS_Destroy(otpos_t handle);

// postag
int OTPOS_Postag_x(otpos_t handle,
        const std::vector<std::string>& sent,
        std::vector<std::string>& pos);

#endif  // end for __OTPOS_DLL_H__
