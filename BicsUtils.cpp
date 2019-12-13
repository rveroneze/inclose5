#include "stdafx.h"
#include "BicsUtils.h"

void openPrintFile(const string &filename)
{
	g_filebics.open(filename);
}

void printBic(const pbic_t &bic, const col_t m, const row_t &n)
{
	pair <double,double> zdc = getZDC(bic->A, bic->sizeA, n, 'a', m);
	if (zdc.first < g_minZDC)
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

pair <double,double> getZDC(const row_t *A, const row_t &sizeA, const row_t &n, const char &option, const col_t &col)
{
/*
option = 'u' => upper bound
option = 'a' => actual value
*/
	for (unsigned short i = 0; i < g_maxLabel; ++i)
	{
		g_contClassBic[i] = 0; // initialize vector
		g_contClassBicUn[i] = 0; // initialize vector
	}
	row_t nUn = 0;
	for (row_t i = 0; i < sizeA; ++i) {
		++g_contClassBic[ g_classes[A[i]] ]; // counting the representativeness of each class label
		if (option == 'u' && col >= g_unav[A[i]])
		{
			++g_contClassBicUn[ g_classes[A[i]] ];
			++nUn;
		}
	}
	
	double zdc;
	pair <double,double> maior;
	maior.first = 0;
	maior.second = 0;
	for (unsigned short i = 0; i < g_maxLabel; ++i)
	{
		row_t ib, ob, ig, og;
		ib = g_contClassBic[i]; // number of samples of label i in the bicluster
		ig = g_contClassGeral[i];  // number of samples of label i in the dataset
		og = n - g_contClassGeral[i];
		if (option == 'u')
		{
			zdc = chi_squared(ib, 0, ig, og);
			if (zdc > maior.second) maior.second = zdc;
			ob = nUn - g_contClassBicUn[i];
		}
		else
		{
			ob = sizeA - g_contClassBic[i];
		}
		zdc = chi_squared(ib, ob, ig, og);
		if (zdc > maior.first) maior.first = zdc;
	}

	return maior;
}

double chi_squared (const row_t &p, const row_t &n, const row_t &pt, const row_t &nt)
{
    double t = nt + pt;
    double p1n = pow(p-((p+n)/t)*pt, 2);
    double p1d = (p+n)/t*pt;
    double p2n = pow(n-((p+n)/t)*nt, 2);
    double p2d = (p+n)/t*nt;
    double p3n = pow(pt-p-(t-(p+n))/t*pt, 2);
    double p3d = (t-(p+n))/t*pt;
    double p4n = pow(nt-n-(t-(p+n))/t*nt,2);
    double p4d = (t-(p+n))/t*nt;
	double p1 = 0, p2 = 0, p3 = 0, p4 = 0;
    
    if (p1d != 0) p1 = p1n / p1d;
    if (p2d != 0) p2 = p2n / p2d;
    if (p3d != 0) p3 = p3n / p3d;    
    if (p4d != 0) p4 = p4n / p4d;

    return (p1 + p2 + p3 + p4) / t;
}
