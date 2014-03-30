#include <stdio.h>

char *str="汉字里有english也能正确显示，hehe";

int main() {
  int i = 0;
  while (str[i] != '\0') {
    if(str[i] < 0) {
       printf("%c%c\n", str[i], str[i+1]);
       i++;
    }
    else {
      printf("%c\n", str[i]);
    }
    i++;
  }
  return 0;
}
