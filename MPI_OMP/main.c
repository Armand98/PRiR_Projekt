#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include <time.h>
#include <math.h>
#include <omp.h>

#define PLOTS 4

long long int QUANTITY_IN_DATASET = 1000000*2;

int MAIN_PLOT_RANK = 0;

void fillArrayWithRandomNumbers(int * array, int minNumber, int maxNumber){
    srand(time(NULL));

    for(long long int i =0;i<QUANTITY_IN_DATASET;i++){
        array[i] = rand() %(maxNumber*10) + minNumber;
    }
}

int sumDigitsInNumber(int number){
    int sum = 0;
    while(number!=0){
        sum+=number%10;
        number/=10;
    }

    return sum;
}

int main(int argc, char **argv) {

    int plotRank, plotsAmount, tag = 0;
    int dataset[QUANTITY_IN_DATASET];
    float resultToSend, resultToReceive;

    long long int  minNumber,  maxNumber, minSumOfDigits;
    minNumber=0;
    maxNumber=1000;
    minSumOfDigits=0;

    double start, end;

    MPI_Init(&argc,&argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD,&plotRank);
    MPI_Comm_size(MPI_COMM_WORLD, &plotsAmount);

    if(plotRank == 0){
        fillArrayWithRandomNumbers(dataset,minNumber, maxNumber);
    }
    if(plotRank == 0){

        start = MPI_Wtime();

        float sumOfAllPlots = 0;

        for(long long int i = MAIN_PLOT_RANK+1;i<plotsAmount;i++){
            MPI_Send(&dataset,QUANTITY_IN_DATASET,MPI_INT,i,tag,MPI_COMM_WORLD);

        }
        for(long long int i = MAIN_PLOT_RANK+1;i<plotsAmount;i++){
            MPI_Recv(&resultToReceive,QUANTITY_IN_DATASET,MPI_INT,i,tag,MPI_COMM_WORLD,&status);
            sumOfAllPlots+=resultToReceive;
        }

        float resultFromAllPlots = sumOfAllPlots/(plotsAmount-1);

        long long int plotSize = ceil(QUANTITY_IN_DATASET/(plotsAmount-1));

        end = MPI_Wtime();


        printf("Czas realizacji dla %d watkow : %f\n",plotsAmount, end-start);
        printf("Wynik działania dla przedziału <%lld,%lld> o minimalnej sumie liczb %lld: %f\n",minNumber, maxNumber,minSumOfDigits,resultFromAllPlots);
    } else {
        long long int plotSize = ceil(QUANTITY_IN_DATASET/(plotsAmount-1));
        long long int sumInPlot = 0;

        MPI_Recv(&dataset,QUANTITY_IN_DATASET,MPI_INT,MAIN_PLOT_RANK,tag,MPI_COMM_WORLD,&status);
        long long int matchedNumbers = 0;

        #pragma omp parallel for reduction(+:sumInPlot) reduction(+:matchedNumbers) num_threads(PLOTS)
        for(long long int i=plotSize*(plotRank-1);i<(plotRank)*plotSize;i++){
            if(dataset[i]<minNumber){
                continue;
            }
            if(maxNumber<dataset[i]){
                continue;
            }

            if(sumDigitsInNumber(dataset[i])<minSumOfDigits){
                continue;
            }
            matchedNumbers++;
            sumInPlot+=dataset[i];
        }

        resultToSend = (sumInPlot/matchedNumbers);

        MPI_Send(&resultToSend,1,MPI_INT,MAIN_PLOT_RANK,tag,MPI_COMM_WORLD);
    }

    MPI_Finalize();
}