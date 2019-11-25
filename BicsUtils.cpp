#include "stdafx.h"
#include "BicsUtils.h"

void openPrintFile(const string &filename)
{
	g_filebics.open(filename);
}

void printBic(const pbic_t &bic, const col_t m)
{
	if (g_minConf > 0 && getMinConf(bic->A, bic->sizeA) < g_minConf)
		return;

	++g_cont;
	g_filebics << "A{" << g_cont << "} = [";
	for (row_t i = 0; i < bic->sizeA; ++i)
		g_filebics << bic->A[i] + 1 << " ";
	g_filebics << "];\nB{" << g_cont << "} = [";
	for (col_t i = 0; i < m; ++i)
	{
		if (bic->B[i])
			g_filebics << i + 1 << " ";
	}
	g_filebics << "];\n";
}

void closePrintFile()
{
	g_filebics.close();
}

double getMinConf(row_t *A, row_t size)
{
// Compute the confidence of a formal concept

	for (unsigned short i = 0; i < g_maxLabel; ++i) g_contClassBic[i] = 0; // initialize vector
	for (row_t i = 0; i < size; ++i) ++g_contClassBic[ g_classes[A[i]] ]; // counting the representativeness of each class label
	
	unsigned int maior = 0, label = 0;
	for (unsigned short i = 0; i < g_maxLabel; ++i)
	{
		if (g_contClassBic[i] > maior)
		{
			maior = g_contClassBic[i];
			label = i;
		}
	}

	if (g_ignoreLabel == label) return 0;
	return maior / (double) size;
}
