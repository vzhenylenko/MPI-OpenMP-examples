#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <omp.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
int main(int argc, char** argv){

int n = atoll(argv[1]);
int nthrd = atoll(argv[2]);

int i, j, k;
double t1, t2;
double** a = new double*[n];
double** b = new double*[n];
double** c = new double*[n];

for(i =0; i<n; i++){
  a[i] = new double[n];
  b[i] = new double[n];
  c[i] = new double[n];
}

for (i=0; i<n; i++)
  for (j=0; j<n; j++)
    a[i][j]=b[i][j]=(i+1)*(j+1);
t1=omp_get_wtime();

omp_set_num_threads(nthrd);
#pragma omp parallel for shared(a, b, c) private(i, j, k)\
  num_threads(nthrd)
for(i=0; i<n; i++){
  for(j=0; j<n; j++){
      c[i][j] = 0.0;
      for(k=0; k<n; k++) c[i][j]+=a[i][k]*b[k][j];
}}

if (n<=10) {
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++)
          cout << c[i][j] << "  ";
          cout << '\n';
  }
}
else
  cout << "Too big" << '\n';

t2=omp_get_wtime();
printf("Time = %lf\n", t2-t1);
}
