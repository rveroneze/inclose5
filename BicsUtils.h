#pragma once

#include "stdafx.h"
#include "globalsv.h"

void openPrintFile(const string &filename);
void printBic(const pbic_t &bic, const col_t m, const row_t &n);
void closePrintFile();
double getZDC(const row_t *A, const row_t &sizeA, const row_t &n, const char &option, const col_t &col);
double chi_squared (const row_t &p, const row_t &n, const row_t &pt, const row_t &nt);
