/* This program assigns */

main () {

  Matrix a = readMatrix ("../samples/my_code_1(1).data" ) ;		 
  Matrix b = readMatrix ("../samples/my_code_1(2).data");

  Int rows_a;
  Int cols_b;
  Int rows_b;
  
  rows_a = numRows(a);
  cols_b = numCols(b);
  rows_b = numRows(b);
  
  Matrix result[rows_a, cols_b] i, j = 0;
  
  Int i;
  Int j;
  Int k;
  
  for (i = 0 : rows_a -1) {
    for (j = 0 : cols_b - 1) {
	  for (k = 0 : rows_b - 1) {
		result[i,j] = result[i,j] + ( a[i,k] * b[k,j]);
	  }  
	}
  }
  
  print (result);
}



