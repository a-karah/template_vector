
#include "tempv_lib.h"
#include <stdio.h>

//GENERATE_VECTOR_HEADER(int, INT)
//GENERATE_VECTOR(int, INT)
GENERATE_TEMP_VECTOR_HEADER(int, INT)
GENERATE_TEMP_VECTOR(int, INT)
GENERATE_TEMP_VECTOR_HEADER(char *, CHRP)
GENERATE_TEMP_VECTOR(char *, CHRP)

 int    main()
 {
    INTtempv_t *vec;
    CHRPtempv_t *cvec;
    cvec = CHRPtempv_new(NULL, 0, NULL);
    vec = INTtempv_new(NULL,0, NULL);
    INTtempv_push(vec, 10);
    CHRPtempv_push(cvec, "hello");
      CHRPtempv_push(cvec, "world");

   printf("%d\n", vec->vector[0]);
   printf("%s\n", cvec->vector[0]);
   printf("%s\n", cvec->vector[1]);

 } 