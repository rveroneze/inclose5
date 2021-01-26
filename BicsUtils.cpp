#include "stdafx.h"
#include "BicsUtils.h"

void openPrintFile(const string &filename)
{
	g_filebics.open(filename);
}

void printBic(const pbic_t &bic, const col_t m, const row_t &n)
{
	if (g_minZDC > 0)
	{
		pair <double,double> zdc = getZDC(bic->A, bic->sizeA, n, 'a', m);
		if (zdc.first < g_minZDC)
		{
			++g_contFails;
			return;
		}
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


/*
option = 'u' => upper bound
option = 'a' => actual value
*/
pair <double,double> getZDC(const row_t *A, const row_t &sizeA, const row_t &n, const char &option, const col_t &col)
{
	row_t *contClassBic = new row_t[g_maxLabel], *contClassBicUn = new row_t[g_maxLabel], *contClassBicAux = new row_t[g_maxLabel];

	// initialize vectors
	for (unsigned short i = 0; i < g_maxLabel; ++i)
	{
		contClassBic[i] = 0;
		contClassBicUn[i] = 0;
		contClassBicAux[i] = 0;
	}
	
	// counting the representativeness of each class label
	row_t nUn = 0;
	for (row_t i = 0; i < sizeA; ++i)
	{
		++contClassBic[ g_classes[A[i]] ];
		if (option == 'u' && col >= g_unav[A[i]])
		{
			++contClassBicUn[ g_classes[A[i]] ];
			++nUn;
		}
	}
	
	pair <double,double> maior;
	maior.first = 0;
	maior.second = 0;
	if (option == 'u')
	{
		for (unsigned short i = 0; i < g_maxLabel; ++i)
		{
			contClassBicAux[i] = contClassBic[i];
			double zdc = chi_squared(contClassBicAux, contClassBicAux[i], n);
			contClassBicAux[i] = 0;
			if (zdc > maior.second) maior.second = zdc;

			row_t aux = contClassBicUn[i];
			contClassBicUn[i] = contClassBic[i];
			zdc = chi_squared(contClassBicUn, nUn - aux + contClassBic[i], n);
			contClassBicUn[i] = aux;
			if (zdc > maior.first) maior.first = zdc;
		}
		maior.first = maior.second;
	}
	else maior.first = chi_squared(contClassBic, sizeA, n);

	delete [] contClassBic;
	delete [] contClassBicUn;
	delete [] contClassBicAux;

	return maior;
}

double chi_squared (const row_t *contClassBic, const row_t &sizeA, const row_t &n)
{
	row_t nout = n - sizeA;

	double soma = 0, Ei1, Ei2;
	for (unsigned short i = 0; i < g_maxLabel; ++i)
	{
		Ei1 = sizeA * g_contClassGeral[i] / (double) n;
		if (Ei1 != 0) soma = soma + pow(contClassBic[i] - Ei1, 2) / Ei1;
		Ei2 = nout * g_contClassGeral[i] / (double) n;
		if (Ei2 != 0) soma = soma + pow(g_contClassGeral[i] - contClassBic[i] - Ei2, 2) / Ei2;
	}

	return soma / n;
}
