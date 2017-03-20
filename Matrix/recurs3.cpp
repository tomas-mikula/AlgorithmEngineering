#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <omp.h>
#include </usr/local/include/valgrind/callgrind.h>
using namespace std;

int maxNumber = 10;

// It is assumed the vector is properly dimensioned
void printMatrix( vector < vector < int > > matrix ) {
	
	vector<int> first = matrix[0];

	int coloumns = matrix.size();
	int rows = first.size();

	printf("\n");

	for (int i = 0; i < coloumns; ++i) {
		
		printf( "| " );

		for (int j = 0; j < rows; ++j) {
			cout << matrix[i][j] ;
            printf( " " );
		}

		printf("| \n");
	}
	printf("\n");
}

//Makes matrix with random entrances, size defined by parameters
vector < vector < int > > makeRandomMatrix(int rows, int coloumns) {

	vector < vector < int > > matrix;

	matrix.resize(rows);
	
	for (int i = 0; i < rows; ++i) {
		matrix[i].resize(coloumns);
	}

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < coloumns; ++j) {
			matrix[i][j] = rand() % maxNumber;
		}
	}

	return matrix;
	
}


vector < vector < int > > makeZeroMatrix(int rows, int coloumns) {

	vector < vector < int > > matrix;

	matrix.resize(rows);
	
	for (int i = 0; i < rows; ++i) {
		matrix[i].resize(coloumns);
	}

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < coloumns; ++j) {
			matrix[i][j] = 0;
		}
	}

	return matrix;
	
}


//Matrix addition, assumes properly dimensioned matrices 
void addMatrix(vector < vector < int > > &matrixFrom, vector < vector < int > > &matrixTo,
int startRows, int startColoumns, int rows, int coloumns) {
	
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < coloumns; ++j) {
			matrixTo[i + startRows][j + startColoumns] += matrixFrom[i][j];
		}
	}
}

void copyMatrix(vector < vector < int > > &matrixFrom, vector < vector < int > > &matrixTo,
int startRows, int endRows, int startColoumns, int endColoumns) {
	
	for (int i = startRows; i < endRows; ++i) {
		for (int j = startColoumns; j < endColoumns; ++j) {
			matrixTo[i - startRows][j - startColoumns] = matrixFrom[i][j];
		}
	}
}

//Basic matrix multiplication, assumes properly dimensioned matrices

vector < vector < int > > multiplyMatrices_basic( vector < vector < int > > matrixLeft,  vector < vector < int > > matrixRight )
{

	vector<int> firstLeft = matrixLeft[0];
	vector<int> firstRight = matrixRight[0];

	int rowsLeft  = matrixLeft.size();
	int coloumnsLeft = firstLeft.size();
	int rowsRight = matrixRight.size();
	int coloumnsRight = firstRight.size();

	int i, j, k;

	vector< vector < int > > res;
	res.resize(rowsLeft );

	// Initialize result matrix to correct size and filled with 0
	for(i = 0; i < rowsLeft ; ++i)
	{
		res[i].resize(coloumnsRight);
		for(j = 0; j < coloumnsRight; ++j)
		{
			res[i][j] = 0;
		}
	} 

	//Actual multiplication begins here
	
	clock_t begin = clock();
    CALLGRIND_ZERO_STATS;
    CALLGRIND_START_INSTRUMENTATION;

	// Multiplying matrices
	//#pragma omp parallel for
	for(i = 0; i < rowsLeft ; ++i)
	{
		for(j = 0; j < coloumnsRight; ++j)
		{
			for(k=0; k < rowsRight; ++k)
			{
				res[i][j] += matrixLeft[i][k] * matrixRight[k][j];
			}
		}
	} 

	// multiplication done
	CALLGRIND_STOP_INSTRUMENTATION;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	int numberOfElements = rowsLeft * coloumnsLeft + rowsRight * coloumnsRight;
    char msg[100];
    sprintf(msg,"%d %f", numberOfElements, elapsed_secs);
    CALLGRIND_DUMP_STATS_AT(msg);

    cout << "\n----Results----- " << endl;
    cout << "Matrix multiplication took: " << elapsed_secs << " seconds" << endl;
	cout << "Matrix A size: " << rowsLeft << "x" << coloumnsLeft << "\n";
	cout << "Matrix B size: " << rowsRight << "x" << coloumnsRight << "\n";
	return res;
}

// A * B, Multiply matrixes multiply matrices recursively, assumes properly dimensioned matrices
void multiplyMatrices_recursive( vector < vector < int > > &matrixLeft,  vector < vector < int > > &matrixRight, vector < vector < int > > &matrixRes, 
 int startLeft_rows, int endLeft_rows, int startLeft_coloumns, int endLeft_coloumns, 
 int startRight_rows, int endRight_rows, int startRight_coloumns, int endRight_coloumns,
 int startRes_rows, int endRes_rows, int startRes_coloumns, int endRes_coloumns) {
	

	int rowsLeft  = endLeft_rows - startLeft_rows;
	int coloumnsLeft = endLeft_coloumns - startLeft_coloumns;
	int coloumnsRight = endRight_coloumns - startRight_coloumns;

	

	/*bool m = (rowsLeft > coloumnsLeft) && (rowsLeft > coloumnsRight);
	bool n = (coloumnsLeft > rowsLeft) && (coloumnsLeft > coloumnsRight);
	bool p = (coloumnsRight > rowsLeft) && (coloumnsRight > coloumnsLeft);*/

	bool m = rowsLeft > 1;
	bool n = (coloumnsLeft > 1) && !m;
	bool p = (coloumnsRight > 1) && !m && !n;

	if (!m && !n && !p) {
		matrixRes[startRes_rows] [startRes_coloumns] += (matrixLeft[startLeft_rows][startLeft_coloumns] * matrixRight[startRight_rows][startRight_coloumns]);
		return;
	}



	int startLeft_rows2 = startLeft_rows;
	int endLeft_rows2 = endLeft_rows; 
	int startLeft_coloumns2 = startLeft_coloumns; 
	int endLeft_coloumns2 = endLeft_coloumns; 
	int startRight_rows2 = startRight_rows; 
	int endRight_rows2 = endRight_rows; 
	int startRight_coloumns2 = startRight_coloumns; 
	int endRight_coloumns2 = endRight_coloumns;
	int startRes_rows2 = startRes_rows; 
	int endRes_rows2 = endRes_rows; 
	int startRes_coloumns2 = startRes_coloumns; 
	int endRes_coloumns2 = endRes_coloumns;

	endLeft_rows = endLeft_rows * !m +  ((startLeft_rows + endLeft_rows) / 2) * m;
	startLeft_rows2 = startLeft_rows * !m + endLeft_rows * m;
	endRes_rows = endRes_rows * !m +  ((startRes_rows + endRes_rows) / 2) * m;
	startRes_rows2 = startRes_rows * !m +  endRes_rows * m;

	endLeft_coloumns = endLeft_coloumns * !n + ((startLeft_coloumns + endLeft_coloumns) / 2) * n;
	startLeft_coloumns2 = startLeft_coloumns * !n + endLeft_coloumns * n;
	endRight_rows = endRight_rows * !n + ((startRight_rows + endRight_rows) / 2) * n;
	startRight_rows2 = startRight_rows * !n + endRight_rows * n;

	endRight_coloumns = endRight_coloumns * !p + ((startRight_coloumns + endRight_coloumns) / 2) * p;
	startRight_coloumns2 = startRight_coloumns * !p + endRight_coloumns * p;
	endRes_coloumns = endRes_coloumns * !p + ((startRes_coloumns + endRes_coloumns) / 2) * p;
	startRes_coloumns2 = startRes_coloumns * !p + endRes_coloumns * p;


	multiplyMatrices_recursive(matrixLeft, matrixRight, matrixRes,
		startLeft_rows, endLeft_rows, startLeft_coloumns, endLeft_coloumns, 
		startRight_rows, endRight_rows, startRight_coloumns, endRight_coloumns,
		startRes_rows, endRes_rows, startRes_coloumns, endRes_coloumns); 
	multiplyMatrices_recursive(matrixLeft, matrixRight, matrixRes,
		startLeft_rows2, endLeft_rows2, startLeft_coloumns2, endLeft_coloumns2, 
		startRight_rows2, endRight_rows2, startRight_coloumns2, endRight_coloumns2,
		startRes_rows2, endRes_rows2, startRes_coloumns2, endRes_coloumns2); 
	
	return;
}

vector < vector < int > > multiplyMatrices_recursive_callgrind( vector < vector < int > > &A,  vector < vector < int > > &B, vector < vector < int > > &res, 
const int &m, const int &n, const int &p) {
	
	clock_t begin = clock();
    CALLGRIND_ZERO_STATS;
    CALLGRIND_START_INSTRUMENTATION;

	multiplyMatrices_recursive(A, B, res, 0, m, 0, n, 0, n, 0, p, 0, m, 0, p);

	CALLGRIND_STOP_INSTRUMENTATION;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	int numberOfElements = m * n + n * p;
    char msg[100];
    sprintf(msg,"5 %d %f", numberOfElements, elapsed_secs);
    CALLGRIND_DUMP_STATS_AT(msg);

	return res;
}


int main( int argc, const char* argv[] ) {

	int i = 5;

	int m = i;
	int n = i;
	int p = i; 
	
	vector< vector < int > > A, B, res;
	// A * B
	// A = m * n, B = n * p
	int maxSize = 1000;
	int increment = 50;
	while (i <= maxSize) {

		/*if (i <= tests/4)
			++m;
		else if (i > tests/4 && i <= (tests/4)*2)
			++n;
		else if (i > (tests/4)*2 && i <= (tests/4)*3)
			++p;
		else {++m;}*/

		switch (i % 3) {
			case 0 : 
				m += increment;
				break;
			case 1 : 
				n += increment;
				break;
			case 2 : 
				p += increment;
				break;
		}
		
		cout << "i: " <<  i << "\n";
		i += 50;
		
		cout << "m: " << m << "\n"; 
		cout << "n: " << n << "\n";
		cout << "p: " << p << "\n";  
		A = makeRandomMatrix(m, n);
		B = makeRandomMatrix(n, p);  

		//printMatrix(A);
		//printMatrix(B);

		cout << "\n \n \n";

		res = makeZeroMatrix(m, p);
		//printMatrix(res);
		multiplyMatrices_recursive_callgrind(A, B, res, m, n, p);
		//res = multiplyMatrices_recursive(A, B, res, 0, m, 0, n, 0, n, 0, p, 0, m, 0, p);
		//res = multiplyMatrices_basic(A, B);

	}
	
/*	A = makeRandomMatrix(2, 4);
	printMatrix(A);
	B = makeRandomMatrix(4, 2);
	printMatrix(B);  
	res = makeZeroMatrix(2, 2);

	multiplyMatrices_recursive(A, B, res, 0, 2, 0, 4, 0, 4, 0, 2, 0, 2, 0, 2);
	printMatrix(res);
	printMatrix(multiplyMatrices_basic(A, B));*/
	
	return 0;
}