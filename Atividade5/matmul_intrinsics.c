#include <stdio.h>
#include <stdlib.h>
// #include <smmintrin.h>
// #include <immintrin.h>
#include <x86intrin.h>

int randomInt() {
  return rand() % 200;
}

int** createMatrix(int rows, int cols) {
  int **mat = (int **)malloc(rows * sizeof(int*));
  for(int i = 0; i < rows; i++) {
    mat[i] = aligned_alloc(32 , sizeof(int) * cols);;
  }
  return mat;
}
 
int main(int argc, char **argv) {
  int m, n, p, q, c, d, k, sum = 0;
 
  // m, n => number of rows/columns for First matrix
  m = atoi(argv[1]);
  n = atoi(argv[1]);

  // p, q => number of rows/columns for Second matrix
  p = atoi(argv[1]);
  q = atoi(argv[1]);
 
  int** first = createMatrix(m, n);
  int** second = createMatrix(p, q);
  int** multiply = createMatrix(m, q);

  for (c = 0; c < m; c++)
    for (d = 0; d < n; d++)
      first[c][d] = randomInt();
 
  if (n != p)
    printf("The multiplication isn't possible.\n");
  else
  {
    for (c = 0; c < p; c++)
      for (d = 0; d < q; d++){
        second[c][d] = randomInt();
      }
 
    for (c = 0; c < m; c++) {
      int *row = multiply[c];
      for (d = 0; d < q; d++) {
        int a = first[c][d];
			  int aux[8] __attribute__((aligned(32))) = {a ,a ,a ,a ,a ,a ,a ,a };
        __m256i vector1 = _mm256_load_si256((__m256i*)aux);
			  int size_p = p-8;
        for (k = 0; k < size_p; k += 8) {
          __m256i vector2 = _mm256_load_si256((__m256i*)(second[d] + k));
          __m256i vector3 = _mm256_load_si256((__m256i*)(row + k));
          __m256i vector4 = _mm256_mullo_epi32(vector2,vector1);
          vector4 = _mm256_add_epi32(vector4 , vector3);
          _mm256_store_si256((__m256i*)(row + k) , vector4);
        }

        for(; k < q ; k++){
				multiply[k] += a * second[d][k];
			}
      }
    }
 
//  Prints the resulting matrix (Uncomment)
    // for (c = 0; c < m; c++) {
    //   for (d = 0; d < q; d++)
    //     printf("%d\t", multiply[c][d]);

    //   printf("\n");
    // }
  }
  printf("Done.");
  return 0;
}
