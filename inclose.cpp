#include "stdafx.h"
#include "inclose.h"

float runInClose(const dataset_t &D, const row_t &n, const col_t &m, const row_t &minRow, const col_t &minCol)
{
	clock_t clocks = clock();
	g_RW = new row_t[n];

	// Creating the supremum
	pbic_t bic = new bic_t;
	bic->A = new row_t[n];
	for (row_t i = 0; i < n; ++i)
		bic->A[i] = i;
	bic->sizeA = n;
	bic->B = new bool[m];
	bic->Z = new bool[m];
	for (col_t i = 0; i < m; ++i)
	{
		bic->B[i] = false;
		bic->Z[i] = false;
	}
	bic->sizeB = 0;
	bic->col = 0;

	InClose(D, m, minRow, minCol, bic, n); // call In-Close

	clocks = clock() - clocks;
	return ((float)clocks) / CLOCKS_PER_SEC;
}

void InClose(const dataset_t &D, const col_t &m, const row_t &minRow, const col_t &minCol, const pbic_t &bic, const col_t &n)
{
	queue<pbic_t> children;

	// Iterating across the attributes
	for (col_t j = bic->col; j < m; ++j)
	{
		if (m - j + bic->sizeB < minCol)
			break;

		if (!bic->B[j] && !bic->Z[j])
		{
			// Computing RW
			row_t sizeRW = 0;
			for (row_t i = 0; i < bic->sizeA; ++i)
			{
				if (D[bic->A[i]][j])
					g_RW[sizeRW++] = bic->A[i];
			}

			// "Main routine"
			if (sizeRW >= minRow)
			{
				col_t fcol;
				if (sizeRW == bic->sizeA)
				{
					bic->B[j] = true;
					++bic->sizeB;
				}
				else if (IsCanonical(D, j, sizeRW, bic, fcol))
				{
					pbic_t child = new bic_t;
					child->sizeA = sizeRW;
					child->A = new row_t[sizeRW];
					for (row_t i = 0; i < sizeRW; ++i)
						child->A[i] = g_RW[i];
					child->col = j + 1;
					children.push(child);
				}
				else if (fcol < bic->col) bic->Z[j] = true;
			}
			else bic->Z[j] = true;
		}
	}

	// Print the formal concept and deallocate the memory of its extent
	if (bic->sizeB >= minCol)
		printBic(bic, m, n);
	delete[] bic->A;

	// Closing the children
	while (!children.empty())
	{
		pbic_t child = children.front();
		child->B = new bool[m];
		child->Z = new bool[m];
		for (col_t j = 0; j < m; ++j)
		{
			child->B[j] = bic->B[j];
			child->Z[j] = bic->Z[j];
		}
		child->B[child->col - 1] = true;
		child->sizeB = bic->sizeB + 1;
		InClose(D, m, minRow, minCol, child, n);
		children.pop();
	}
	delete[] bic->B;
	delete[] bic->Z;
	delete bic;
}

bool IsCanonical(const dataset_t &D, const col_t &y, const row_t &sizeRW, const pbic_t &bic, col_t &fcol)
{
	row_t i;
	for (col_t j = 0; j < y; ++j)
	{
		if (!bic->B[j] && !bic->Z[j])
		{
			for (i = 0; i < sizeRW; ++i)
				if (!D[g_RW[i]][j])
					break;
			fcol = j; // if IsCanonical==false, it is used to keep the column where the test fail
			if (i == sizeRW)
				return false;
		}
	}
	return true;
}
