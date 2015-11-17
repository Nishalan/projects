#include <stdio.h>
#include <string>
#include <iostream>
#include "Matrix.h"
#include <stdlib.h>
#include "iostream"
#include "fstream"
using namespace std;


Matrix::Matrix(int rows_, int cols_){

	rows=rows_;
	cols=cols_;
	
	data = (float**)malloc(rows * sizeof(float *));
	if (data)
	{
		for (int i = 0; i < rows; i++)
			data[i] = (float*)malloc(cols * sizeof(float));
	}
}

Matrix::Matrix(const Matrix& m) {
	rows= m.rows;
	cols= m.cols;

	data = (float**)malloc(rows * sizeof(float *));
	if (data)
	{
		for (int i = 0; i < rows; i++)
			data[i] = (float*)malloc(cols * sizeof(float));
	}

	for (int i = 0; i < rows; i ++ ) 
	{
        for (int j = 0; j < cols; j ++ ) 
		{
       		data[i][j] = m.data[i][j];
        }
    }


	
}

float* Matrix::access(const int i, const int j) const {
	return &data[i][j];
}

//this thing below will allow "cout<< m <<endl" to print the matrix just like that

std::ostream& operator<<(std::ostream &os, Matrix &m){
    if(m.data){
        os << m.numRows() << " " << m.numCols() << std::endl; //as in the sample file, we print the row and col on top

        for(int i = 0; i < m.numRows(); i++){
            for(int j = 0; j < m.numCols(); j++)
                os << *(m.access(i,j)) << "  ";
            os << std::endl;
        }
    }
    else
        os << "Matrix is not valid"; 
    return os;
}

Matrix Matrix::readMatrix ( std::string filename ){
    
    ifstream myfile(filename.c_str());
    int t_rows, t_cols;; //temporarily stores rows and cols , gets it from txt file

    if(!myfile) // Test to see if file is open or not
    {
                cout<<"Error opening output file"<<endl;
    }
    
    myfile>>t_rows;
    myfile>>t_cols;

    Matrix m(t_rows,t_cols); //creates the Matrix object, currently blank

    for (int i = 0; i < t_rows; i ++ ) {
        for (int j = 0; j < t_cols; j ++ ) {
            myfile>>(*(m.access(i, j)));
        }
    }

    
    return m;
}
