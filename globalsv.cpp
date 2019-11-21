#include "stdafx.h"
#include "globalsv.h"

row_t *g_RW;  // // vector to store the extent of a possible formal concept
unsigned g_cont = 0; // number of biclusters in the output
ofstream g_filebics; // pointer to the output file
