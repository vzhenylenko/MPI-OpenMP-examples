#include <iostream>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <mpi.h>


double x[5] = {0, 0, 0, 0, 0};
double xmin[5] = {-1, -1, -1, -1, -1};
double xmax[5] = {1, 1, 1, 1, 1};


double f(double* v)
{
    double r = 0;
    for (size_t i = 0; i < 4; ++i)
    {
        r += pow(v[i], 2);
    }
    if (r < 1)
    {
        return 0.0;
    }
    else
    {
        return 1.0;
    }
}



double MC_Integrate(int N, double* xmin, double* xmax, int seed)
{
    double result = 0;
    srand(time(NULL) + seed);

    for(int i = 0; i != N; ++i)
    {
        for (int j = 0; j != 5; ++j)
        {
            x[j] = (double)rand() / (double)RAND_MAX * (xmax[j] - xmin[j]) + xmin[j];
        }
        result += f(x);
    }

    return result;

}  
int main(int argc, char *argv[]){

    long long N = atoi(argv[1]);
    int rank, size;
    double MC_result = 0, MC_mean = 0, V = 1.0;

    double True_value = 22.13039559;



    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);



    double MC_start = MPI_Wtime();
    double MC_local = MC_Integrate(N / size, xmin, xmax, rank);


    MPI_Reduce(&MC_local, &MC_mean, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    MC_mean /= N;

    for (int i = 0; i < 5; ++i)
    {
        V *= (xmax[i] - xmin[i]);
    }

    MC_result = V * MC_mean;

    if (rank == 0)
    {
        std::cout << "True Integral  = " << True_value << std::endl;
        std::cout << "MC calculated = " << MC_result << std::endl;
        std::cout << "Error =  " << MC_result - True_value << " Time = " << MPI_Wtime() - MC_start << std::endl;

    }

    MPI_Finalize();
    return 0;

}
