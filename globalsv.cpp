#include "stdafx.h"
#include "globalsv.h"

row_t *g_RW;  // // vector to store the extent of a possible formal concept
unsigned g_cont = 0; // number of biclusters in the output
ofstream g_filebics; // pointer to the output file

// ----- Variables for the search using class labels -----
unsigned short *g_classes; // vector to store the class label of each object
unsigned short g_maxLabel; // maximum label
double g_minZDC = 0; // threshold of the ZDC measure
row_t *g_contClassGeral; // stores the number of samples of each class label
row_t *g_contClassBic; // stores the number of samples of each class label in a bicluster
unsigned g_contFails = 0; // number of biclusters that were mined but discarded
// -------------------------------------------------------