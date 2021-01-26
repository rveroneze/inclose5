#pragma once

#include "stdafx.h"

extern row_t *g_RW;
extern unsigned g_cont;
extern ofstream g_filebics;

// ----- Variables for the search using class labels -----
extern unsigned short *g_classes;
extern unsigned short g_maxLabel;
extern double g_minZDC;
extern row_t *g_contClassGeral;
extern unsigned g_contFails;
extern col_t *g_unav;
// -------------------------------------------------------