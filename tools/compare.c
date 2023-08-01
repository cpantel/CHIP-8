#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// taken from https://rosettacode.org/wiki/Repeat_a_string#C
char * string_repeat( int n, const char * s ) {
  size_t slen = strlen(s);
  char * dest = malloc(n*slen+2);

  int i; char * p;
  for ( i=0, p = dest; i < n; ++i, p += slen ) {
    memcpy(p, s, slen);
  }
  *p = '\0';
  return dest;
}

int main(int argc, char* argv[]) {
    int count = 0;
    FILE* fhA = fopen(argv[1],"r");
    FILE* fhB = fopen(argv[2],"r");

    char *lineA = NULL;
    char *lineB = NULL;

    size_t lenA = 0;
    size_t lenB = 0;

    while(getline(&lineA, &lenA, fhA) != -1) {
       getline(&lineB, &lenB, fhB);
       int cmp = strcmp(lineA,lineB);
       if ( cmp != 0) {
         printf("expected: %s", lineA);
         char * rep = string_repeat(cmp, " ");
         printf("%s XXX %d\n", rep, count);
	 free(rep);
         printf("got     : %s\n", lineB);
       } else {
         printf("good    : %s", lineA);

       }
       ++count;
     }
 
     fclose(fhA);
     fclose(fhB);

     free(lineA);
     free(lineB);
}

