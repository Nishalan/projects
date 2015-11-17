/* This program generates all prime numbers from 2-500 */
/* By convention, we know that there are 95 prime numbers in that range */

main () { 
  Int curr;
  curr = 2;
  
  while(curr <=500)
  {
    Int x;
    Int limit;
    limit = ceil(curr/2);

    Bool isprime;
    isprime = True;

    for( x=2 : limit)
    {
      Int pre;
      pre = floor(curr / x);
      if( curr - x * pre == 0) isprime = False;
    }
    
    if ( isprime ) {print(curr); print("\n"); }
        
    curr = curr + 1;
  }

    


    
    
  	
}
