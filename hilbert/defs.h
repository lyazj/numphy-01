#pragma once

#define number double

typedef int solve_t(int n, int m, number A[n][m]);

// gem.c
solve_t gem;

// cholesky.c
solve_t cholesky;
solve_t cdm;
