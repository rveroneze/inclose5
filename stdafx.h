// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <stdio.h>



// TODO: reference additional headers your program requires here
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <queue>
#include <math.h>

using namespace std;

typedef bool data_t;
typedef data_t** dataset_t;

typedef unsigned row_t;
typedef unsigned short col_t;

struct bic_t {
	row_t *A;
	row_t sizeA;
	bool *B, *Z;
	col_t sizeB;
	col_t col;
};

typedef bic_t *pbic_t;
