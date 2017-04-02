int fib(int n){
  int result=0;
  int i=1;
  int fib0=0;
  int fib1=1;

  if(n==0){
    return 0;
  } 
  else if(n==1){
    return 1;
  }
  while(i != n){
    result= fib0 + fib1;
    fib0=fib1;
    fib1=result;
    ++i;
  }
  return result;
}

int main(){
  int n=9;
  int result = fib(n);
  return 0;
}
