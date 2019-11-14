#include "stdafx.h"
#include "globalsv.h"

row_t *g_RW;  // vetor para guardar RW
unsigned g_cont = 0; // contador de biclusters validos
ofstream g_filebics; // pointeiro para o arquivo que vai guardar os bics
