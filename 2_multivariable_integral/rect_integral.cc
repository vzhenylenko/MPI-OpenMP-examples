#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <sys/resource.h>


const double A[5] = {-1, -1, -1, -1, -1};
const double B[5] = {1, 1, 1, 1, 1};


const double xmin[5] = {-1, -1, -1, -1, -1};
const double xmax[5] = {1, 1, 1, 1, 1};

struct rlimit v;

double func(double *x){
        double r =  x[0]*x[0] + x[1]*x[1] + x[2]*x[2] + x[3]*x[3];
        if(r < 1)
        {
                return 0;
        }
        else
        {
                return 1;
        }
}


int main(int argc, char** argv) {
        int size, rank;
        long long N = atoll(argv[1]);
        double Sum, GSum = 0, time1, time2, timeL, timeI, timeT;

        double True_value = 22.13039559;

        int NN = (int)pow(N, 1./5.);
        MPI_Init(NULL, NULL);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        double Al[5];
        double Bl[5];

        double time_start = MPI_Wtime();

        double X[5] = {0, 0, 0, 0, 0};

        Sum = 0;
        for (int i = NN * rank / size; i != NN * (rank + 1) / size; ++i)
        {
            X[0] = xmin[0] + (i + 0.5) * (xmax[0] - xmin[0]) / NN;
            for (int j = 0; j != NN; ++j)
            {
                X[1] = xmin[1] + (j + 0.5) * (xmax[1] - xmin[1]) / NN;
                for (int k = 0; k != NN; ++k)
                {
                    X[2] = xmin[2] + (k + 0.5) * (xmax[2] - xmin[2]) / NN;
                    for (int l = 0; l != NN; ++l)
                    {
                        X[3] = xmin[3] + (l + 0.5) * (xmax[3] - xmin[3]) / NN;
                        for (int q = 0; q != NN; ++q)
                        {
                            X[4] = xmin[4] + (q + 0.5) * (xmax[4] - xmin[4]) / NN;
                            Sum += func(X);
                       }
                    }
                }
            }
        }

        for (int i = 0; i != 5; ++i)
        {

            Sum *= (xmax[i] - xmin[i]) / NN;
        }


        MPI_Reduce(&Sum, &GSum, 1, MPI_DOUBLE_PRECISION, MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0) {
                            std::cout << "True Integral  = " << True_value << std::endl;
                            std::cout << "Rec. calculated = " << GSum << std::endl;
                            std::cout << "Error =  " << GSum - True_value << " Time = " << MPI_Wtime() - time_start << std::endl;;
                        }

        MPI_Finalize();
        return 0;
}
