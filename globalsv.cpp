#include "stdafx.h"
#include "globalsv.h"

unsigned short g_output = 1; // 1 - matlab; 2 - python

row_t *g_RW;  // // vector to store the extent of a possible formal concept
unsigned g_cont = 0; // number of biclusters in the output
ofstream g_filebics; // pointer to the output file

// ----- Variables for the search using class labels -----
unsigned short *g_classes; // vector to store the class label of each object
unsigned short g_maxLabel; // maximum label
double g_minConf = 0; // confidence threshold
row_t *g_contClassBic; // stores the number of samples of each class label in a bicluster
unsigned short g_ignoreLabel; // biclusters that represents label g_ignoreLabel will not be outputted
// -------------------------------------------------------
