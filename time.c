#include<stdio.h>
#include<time.h>
int main() {
  int i;
  for(i = 0; i < 1000000000; i++)
    ;
  double elapsed = clock() / (double)CLOCKS_PER_SEC;
  printf("Time elapsed: %.2lf seconds.\n", elapsed);
}
