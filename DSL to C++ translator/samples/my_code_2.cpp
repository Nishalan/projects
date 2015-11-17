#include <iostream>
#include <math.h>
#include "Matrix.h"
using namespace std;
 
 int main(){
	int curr;
	curr=2;
	while(curr<=500)
{

	int x;
	int limit;
	limit=ceil(curr/2);
	bool isprime;
	isprime=true;
	for(x=2;x<=limit;x ++){

	int pre;
	pre=floor(curr/x);
	if(curr-x*pre==0)
isprime=false;


	}
	if(isprime)
{

	cout<<curr;
	cout<<"\n";

	}

	curr=curr+1;

	}
}

