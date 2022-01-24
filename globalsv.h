#pragma once

#include "stdafx.h"

extern unsigned short g_output;

extern row_t *g_RW;
extern unsigned g_cont;
extern ofstream g_filebics;

// ----- Variables for the search using class labels -----
extern unsigned short *g_classes;
extern unsigned short g_maxLabel;
extern double g_minConf;
extern row_t *g_contClassBic;
extern unsigned short g_ignoreLabel;
// -------------------------------------------------------
