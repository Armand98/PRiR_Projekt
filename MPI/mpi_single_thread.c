#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include <time.h>
#include <math.h>

int QUANTITY_IN_DATASET = 2000000;

int MAIN_PLOT_RANK = 0;

void fillArrayWithRandomNumbers(int * array, int minNumber, int maxNumber){
    srand(time(NULL));

    int minimalValueInDataset = 1;
    int maximalValueInDataset = 10000000;
    int quantityOfDataset = 5000000;

    for(unsigned long int i =0;i<QUANTITY_IN_DATASET;i++){
        int random = rand();
        array[i] = (abs(random)+minNumber)%maxNumber;
    }
}

int sumDigitsInNumber(int number){
    unsigned long sum = 0;
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

    int  minNumber,  maxNumber, minSumOfDigits;
    minNumber=1000;
    maxNumber=2000000;
    minSumOfDigits=15;

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

        int plotSize = QUANTITY_IN_DATASET;
        int sumInPlot = 0;

        int matchedNumbers = 0;

        for(int i=0;i<plotSize;i++){
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

        end = MPI_Wtime();
        printf("ForGrep%f\n", (end-start)/1000);
    }

    MPI_Finalize();
}