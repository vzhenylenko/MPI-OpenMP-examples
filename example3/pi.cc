#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <omp.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
double f(double y) {return(4.0/(1.0+y*y));}
int main(int argc, char** argv)
{
double w, x, sum, pi;
int i;
long long n = atoll(argv[1]);
w = 1.0/n;
int nthrd = atoll(argv[2]);
sum = 0.0;


double start_time = omp_get_wtime();

omp_set_num_threads(nthrd);
#pragma omp parallel for private(x) shared(w)\
reduction(+:sum)
for(i=0; i < n; i++)
{
x = w*(i-0.5);
sum = sum + f(x);
}
pi = w*sum;
// printf("pi = %f\n", pi);
cout<<" Number of threads: "<< omp_get_max_threads()<<" Max number of proc.: "<<omp_get_num_procs()<<endl;
cout.precision(10);
double end_time = omp_get_wtime()-start_time;
cout << " Value = "  << pi<< endl;
cout << " Error = " << pi-3.1415926535 << endl;
cout <<" Time = "<<end_time << endl;
}
