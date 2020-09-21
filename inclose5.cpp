#include "stdafx.h"
#include "globalsv.h"
#include "inclose.h"
#include "BicsUtils.h"

bool readDataset(const string &dataSetName, dataset_t &matrix, row_t &n, col_t &m);
void printData(const dataset_t &matrix, const row_t &n, const col_t &m);
bool readClassLabels(const string &fileName, const row_t &n);


int main(int argc, char* argv[])
{
	if (argc != 7)
	{
		cout << "\n!!! Wrong Arguments !!!" << endl << endl;
		cout << "List of the arguments:" << endl;
		cout << "1) Dataset's filename;" << endl;
		cout << "2) minRow;" << endl;
		cout << "3) minCol;" << endl;
		cout << "4) Output filename for the list of biclusters;" << endl;
		cout << "5) Class labels' filename;" << endl;
		cout << "6) Minimum ZDC value [0,1];" << endl;
		exit(1);
	}

	dataset_t matrix; // pointer to the dataset
	row_t n; // number of dataset's rows
	col_t m; // number of dataset's columns

	row_t minRow = atoi(argv[2]);
	col_t minCol = atoi(argv[3]);
	float tempo;

	cout << "\nArguments: " << endl;
	cout << "Dataset's filename: " << argv[1] << endl;
	cout << "minRow: " << minRow << endl;
	cout << "minCol: " << minCol << endl;
	cout << "File with the list of bicluster: " << argv[4] << endl;
	cout << "Class labels' filename: " << argv[5] << endl;
	cout << "Minimum ZDC: " << argv[6] << endl;

	if (!readDataset(argv[1], matrix, n, m))
	{
		cout << "\nDataset was not loaded!";
		exit(1);
	}
	printf("\nDataset loaded: %dx%d\n\n", n, m);
	//printData(matrix, n, m);

	// Le as classes dos objetos
	g_classes = new unsigned short[n];
	if (!readClassLabels(argv[5], n))
	{
		cout << "\nClass labels' file was not loaded!";
		exit(1);
	}
	printf("Class labels loaded\n\n");
	g_minZDC = atof(argv[6]);

	openPrintFile(argv[4]);
	cout << "\nRunning..." << endl;
	tempo = runInClose(matrix, n, m, minRow, minCol);
	closePrintFile();

	cout << "\nRuntime = " << tempo << endl;
	cout << "Number of biclusters in the output = " << g_cont << endl;
	cout << "Number of discarded biclusters = " << g_contFails << endl;

	return 0;
}

bool readDataset(const string &dataSetName, dataset_t &matrix, row_t &n, col_t &m)
{
	ifstream myStream;
	myStream.open(dataSetName, ifstream::in);

	if (!myStream.is_open())
		return false;

	//Discovering the number of rows
	n = count(istreambuf_iterator<char>(myStream), istreambuf_iterator<char>(), '\n');

	//Discovering the number of columns
	data_t dbltmp;
	string line;
	m = 0;
	myStream.seekg(0);
	getline(myStream, line);
	stringstream stream(line);
	while (stream.good())
	{
		stream >> dbltmp;
		++m;
	}

	//Allocating memory
	matrix = new data_t*[n];
	for (row_t i = 0; i < n; ++i)
		matrix[i] = new data_t[m];

	//Storing the data
	myStream.seekg(0);
	for (row_t i = 0; i < n; ++i)
	{
		for (col_t j = 0; j < m; ++j)
			myStream >> matrix[i][j];
	}

	myStream.close();
	return true;
}

void printData(const dataset_t &matrix, const row_t &n, const col_t &m)
{
	for (row_t i = 0; i < n; ++i)
	{
		for (col_t j = 0; j < m; ++j)
			cout << matrix[i][j] << '\t';
		cout << endl;
	}
}

bool readClassLabels(const string &fileName, const row_t &n)
{
	// Read tha class label of each object, and
	// set g_maxLabel, and g_contClassGeral, and allocate g_contClassBic

	g_maxLabel = 0;

	ifstream myStream;
	myStream.open(fileName, ifstream::in);

	if (!myStream.is_open())
		return false;

	//Storing the data
	myStream.seekg(0);
	for (row_t i = 0; i < n; ++i)
	{
		myStream >> g_classes[i];
		if (g_classes[i] > g_maxLabel) g_maxLabel = g_classes[i];
	}

	myStream.close();
	++g_maxLabel;


	g_contClassGeral = new row_t[g_maxLabel];
	for (unsigned short i = 0; i < g_maxLabel; ++i) g_contClassGeral[i] = 0; // initialize vector
	for (row_t i = 0; i < n; ++i) ++g_contClassGeral[ g_classes[i] ]; // counting the representativeness of each class label
	g_contClassBic = new row_t[g_maxLabel];
	g_contClassBicUn = new row_t[g_maxLabel];

	return true;
}
