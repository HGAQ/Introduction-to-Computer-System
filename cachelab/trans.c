/*
刘松瑞 2100011819
本代码实现了对不同长度的矩阵的转置，具体操作方法有图例可得

 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"
#include "contracts.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. The REQUIRES and ENSURES from 15-122 are included
 *     for your convenience. They can be removed if you like.
 */
/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */
void solve_32_32(int M, int N, int A[N][M], int B[M][N]);
void solve_64_64(int M, int N, int A[N][M], int B[M][N]);
void solve_60_68(int M, int N, int A[N][M], int B[M][N]);
 /*
  * trans - A simple baseline transpose function, not optimized for the cache.
  */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
 

    REQUIRES(M > 0);
    REQUIRES(N > 0);
    switch (M)
    {
    case 32:
        solve_32_32(M, N, A, B);
        break;
    case 64:
        solve_64_64(M, N, A, B);
        break;
    case 60:
        solve_60_68(M, N, A, B);
        break;
    }
    ENSURES(is_transpose(M, N, A, B));
}


void solve_32_32(int M, int N, int A[N][M], int B[M][N]){
    int i,j,a_row,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7,tmp8;
    for (i = 0; i < 32; i+=8) {
        for (j = 0; j < 32; j+=8) {
            for (a_row = 0; a_row < 8; ++a_row, ++i){
                //1 2 3 4 5 6 7 8          1 o o o o o o o  set 4
                //o o o o o o o o          2 o o o o o o o  set 8
                //o o o o o o o o          3 o o o o o o o  set 12
                //o o o o o o o o    =>    4 o o o o o o o  set 16
                //o o o o o o o o          5 o o o o o o o  set 20
                //o o o o o o o o          6 o o o o o o o  set 24
                //o o o o o o o o          7 o o o o o o o  set 28
                //o o o o o o o o          8 o o o o o o o  set 32
                tmp1 = A[i][j];
                tmp2 = A[i][j+1];
                tmp3 = A[i][j+2];
                tmp4 = A[i][j+3];
                tmp5 = A[i][j+4];
                tmp6 = A[i][j+5];
                tmp7 = A[i][j+6];
                tmp8 = A[i][j+7];
                B[j][i] = tmp1;
                B[j+1][i] = tmp2;
                B[j+2][i] = tmp3;
                B[j+3][i] = tmp4;
                B[j+4][i] = tmp5;
                B[j+5][i] = tmp6;
                B[j+6][i] = tmp7;
                B[j+7][i] = tmp8;
            }
            i -= 8;
        }
    }
}

void solve_64_64(int M, int N, int A[N][M], int B[M][N]){
    int i,j,a_row,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7,tmp8;
    for (i = 0; i < 64; i+=8) {
        for (j = 0; j < 64; j+=8) {
            for (a_row = 0; a_row < 4; ++a_row, ++i){
                //1 2 3 4 5 6 7 8          1 . . . 5 . . .  set 4
                //. . . . . . . .          2 . . . 6 . . .  set 12
                //. . . . . . . .          3 . . . 7 . . .  set 20
                //. . . . . . . .    =>    4 . . . 8 . . .  set 28
                //o o o o o o o o          o o o o o o o o  set 4
                //o o o o o o o o          o o o o o o o o  set 12
                //o o o o o o o o          o o o o o o o o  set 20
                //o o o o o o o o          o o o o o o o o  set 28
                tmp1 = A[i][j];
                tmp2 = A[i][j+1];
                tmp3 = A[i][j+2];
                tmp4 = A[i][j+3];
                tmp5 = A[i][j+4];
                tmp6 = A[i][j+5];
                tmp7 = A[i][j+6];
                tmp8 = A[i][j+7];
                B[j][i] = tmp1;
                B[j+1][i] = tmp2;
                B[j+2][i] = tmp3;
                B[j+3][i] = tmp4;
                B[j][i+4] = tmp5;
                B[j+1][i+4] = tmp6;
                B[j+2][i+4] = tmp7;
                B[j+3][i+4] = tmp8;
            }
            i -= 4;
            for (a_row = 0; a_row < 4; ++a_row,j++){
                //o o o o o o o o          o o o o 1 . . .  set 4 
                //o o o o o o o o          o o o o 2 . . .  set 12 
                //o o o o o o o o          o o o o 3 . . .  set 20 
                //o o o o o o o o    =>    o o o o 4 . . .  set 28 
                //1 2 3 4 o o o o          5 6 7 8 o o o o  set 4
                //. . . . o o o o          . . . . o o o o  set 12
                //. . . . o o o o          . . . . o o o o  set 20
                //. . . . o o o o          . . . . o o o o  set 28
                
                tmp1 = A[i+4][j];
                tmp2 = A[i+5][j];
                tmp3 = A[i+6][j];
                tmp4 = A[i+7][j];
                tmp5 = B[j][i+4];
                tmp6 = B[j][i+5];
                tmp7 = B[j][i+6];
                tmp8 = B[j][i+7];
                B[j][i+4] = tmp1;
                B[j][i+5] = tmp2;
                B[j][i+6] = tmp3;
                B[j][i+7] = tmp4;
                B[j+4][i+0] = tmp5;
                B[j+4][i+1] = tmp6;
                B[j+4][i+2] = tmp7;
                B[j+4][i+3] = tmp8;
            }
            j -= 4;
            for (int a_row = 0; a_row < 4; ++a_row, ++i){
                //o o o o o o o o          o o o o o o o o  set 4
                //o o o o o o o o          o o o o o o o o  set 12
                //o o o o o o o o          o o o o o o o o  set 20
                //o o o o o o o o   =>     o o o o o o o o  set 28
                //o o o o 1 2 3 4          o o o o 1 . . .  set 4
                //o o o o . . . .          o o o o 2 . . .  set 12
                //o o o o . . . .          o o o o 3 . . .  set 20
                //o o o o . . . .          o o o o 4 . . .  set 28
                int tmp1 = A[i+4][j+4];
                int tmp2 = A[i+4][j+5];
                int tmp3 = A[i+4][j+6];
                int tmp4 = A[i+4][j+7];
                B[j+4][i+4] = tmp1;
                B[j+5][i+4] = tmp2;
                B[j+6][i+4] = tmp3;
                B[j+7][i+4] = tmp4;
            }
            i -= 4;

        }
    }
}

void solve_60_68(int M, int N, int A[N][M], int B[M][N])
{
    for (int i = 0; i < 68; i += 24){
        for (int j = 0; j < 60; j += 4){
            for(int k = i; k < 68 && k < i+24 ; k++){
                for(int l = j ; l < 60 && l < j+4;l++){
                    B[l][k] = A[k][l];
                }
            }
        }
    }
}
/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register any additional transpose functions */
    registerTransFunction(transpose_submit, transpose_submit_desc);

}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

