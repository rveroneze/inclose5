#include "stdafx.h"
#include "globalsv.h"
#include "inclose.h"
#include "BicsUtils.h"

bool readDataset(const string &dataSetName, dataset_t &matrix, row_t &n, col_t &m);
void printData(const dataset_t &matrix, const row_t &n, const col_t &m);
bool readClassLabels(const string &fileName, const row_t &n);
bool readConfigFile();


int main(int argc, char* argv[])
{
	if (argc < 5 || argc == 7 || argc > 8)
	{
		cout << "\n!!! Wrong Arguments !!!" << endl << endl;
		cout << "List of the arguments:" << endl;
		cout << "1) Dataset's filename;" << endl;
		cout << "2) minRow;" << endl;
		cout << "3) minCol;" << endl;
		cout << "4) Output filename for the list of biclusters;" << endl;
		cout << "5) Class labels' filename (optional);" << endl;
		cout << "6) Confidence [0,1] (when using class labels);" << endl;
		cout << "7) Ignore biclusters with label x = ? (when using class labels);" << endl;
		exit(1);
	}

	if (!readConfigFile())
	{
		cout << "\nConfiguration file was not loaded.\n";
		cout << "Default configuration:" << endl;
		cout << "Output format (1 - matlab; 2 - python): " << g_output << endl;
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
	if (argc > 5)
	{
		cout << "Class labels' filename: " << argv[5] << endl;
		cout << "Confidence: " << argv[6] << endl;
		cout << "Ignore biclusters with label x = "  << argv[7] << endl;
	}

	if (!readDataset(argv[1], matrix, n, m))
	{
		cout << "\nDataset was not loaded!";
		exit(1);
	}
	printf("\nDataset loaded: %dx%d\n\n", n, m);
	//printData(matrix, n, m);

	if (argc > 5)
	{
		// Le as classes dos objetos
		g_classes = new unsigned short[n];
		if (!readClassLabels(argv[5], n))
		{
			cout << "\nClass labels' file was not loaded!";
			exit(1);
		}
		printf("Class labels loaded\n\n");
		
		g_minConf = atof(argv[6]);
		g_ignoreLabel = atoi(argv[7]);
	}

	openPrintFile(argv[4]);
	cout << "\nRunning..." << endl;
	tempo = runInClose(matrix, n, m, minRow, minCol);
	closePrintFile();

	cout << "\nRuntime = " << tempo << endl;
	cout << "Number of biclusters = " << g_cont << endl;

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
	// set g_maxLabel, and allocate g_contClassBic

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

	g_contClassBic = new unsigned short[g_maxLabel];

	return true;
}

bool readConfigFile()
{
	unordered_map<string,string> params;
	ifstream myStream;
	string line;

	myStream.open("config.txt", ifstream::in);

	if (!myStream.is_open())
		return false;
		

	myStream.seekg(0);
	while (myStream.good())
	{
		getline(myStream, line);
		size_t found = line.find("=");
		if (found!=string::npos)
		{
			params[line.substr(0,found)] = line.substr(found+1);
		}
	}

	cout << "myparams contains:" << endl;
	for (auto& x: params)
		cout << x.first << ": " << x.second << endl;
	
	// Setando as variaveis com os parametros
	if (params["OUT"].compare("matlab") == 0)
		g_output = 1;
	else
		g_output = 2;

	myStream.close();
	return true;
}