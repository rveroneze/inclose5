#include "stdafx.h"
#include "BicsUtils.h"

void openPrintFile(const string &filename)
{
	g_filebics.open(filename);
}

void printBic(const pbic_t &bic, const col_t &m, const row_t &n)
{
	if (g_minZDC > 0 && getZDC(bic->A, bic->sizeA, n) < g_minZDC)
	{
		++g_contFails;
		return;
	}

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

double getZDC(const row_t *A, const row_t &sizeA, const row_t &n)
{
	for (unsigned short i = 0; i < g_maxLabel; ++i) g_contClassBic[i] = 0; // initialize vector
	for (row_t i = 0; i < sizeA; ++i) ++g_contClassBic[ g_classes[A[i]] ]; // counting the representativeness of each class label

	return chi_squared(sizeA, n);
}

double chi_squared (const row_t &sizeA, const row_t &n)
{
	row_t nout = n - sizeA;

	double soma = 0, Ei1, Ei2;
	for (unsigned short i = 0; i < g_maxLabel; ++i)
	{
		Ei1 = sizeA * g_contClassGeral[i] / (double) n;
		if (Ei1 != 0) soma = soma + pow(g_contClassBic[i] - Ei1, 2) / Ei1;
		Ei2 = nout * g_contClassGeral[i] / (double) n;
		if (Ei2 != 0) soma = soma + pow(g_contClassGeral[i] - g_contClassBic[i] - Ei2, 2) / Ei2;
	}

	return soma / n;
}
