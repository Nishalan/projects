#include <iostream>
#include <math.h>
#include "Matrix.h"
using namespace std;
 
 int main(){
	Matrix m ( Matrix::readMatrix("../samples/sample_8.data"));
	cout<<m;
}

