#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "mat.h"

double mmult_simd(double *c, double *a, int aRows, int aCols, double *b, int bRows, int bCols) {

  int i, j, k, l = 0;
  
  for (i = 0; i < aRows; i++) {
    for (j = 0; j < bCols; j++)
      c[i * bCols + j] = 0;
    for (k = 0; k < aRows; k++) {
      for (l = 0; l < bCols; l++) {
        c[i * bCols + l] += a[i * aRows + k] * b[k * bCols + l];
      }
    }
  }
  return 0;
}