#include <iostream>
#include <math.h>
#include "Matrix.h"
using namespace std;
 
 int main(){
	Matrix a ( Matrix::readMatrix("../samples/my_code_1(1).data"));
	Matrix b ( Matrix::readMatrix("../samples/my_code_1(2).data"));
	int rows_a;
	int cols_b;
	int rows_b;
	rows_a=a.numRows();
	cols_b=b.numCols();
	rows_b=b.numRows();
	Matrix result( rows_a , cols_b); 
	for( int i = 0;i < rows_a ; i ++ ) { 
		for(int j = 0; j < cols_b ; j ++ ){
			*(result.access(i , j )) = 0;
		}
	}
	int i;
	int j;
	int k;
	for(i=0;i<=rows_a-1;i ++){

	for(j=0;j<=cols_b-1;j ++){

	for(k=0;k<=rows_b-1;k ++){

	*( result.access(i,j )) = *( result.access(i,j ))+(*( a.access(i,k ))**( b.access(k,j ))) ;

	}

	}

	}
	cout<<result;
}

