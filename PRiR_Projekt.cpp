/*
* Zadanie 2.
* Dany jest zbiór liczb całkowitych przynajmniej 5-cyfrowych. 
* Oblicz średnią arytmetyczną  liczb spełniających warunki: 
* liczba należy do zadanego przedziału oraz suma  cyfr liczby jest większa od ustalonej wartości.
*/

#include <iostream>
#include <omp.h>
#include <vector>
#include <string>
#include <time.h>

std::vector<int> generateRandomNumbersInRange(int minNumber, int maxNumber, int quantity)
{
    srand(time(NULL));
    std::vector<int> numbers;

    for (int i = 1; i <= quantity; i++)
        numbers.push_back(rand() % maxNumber + minNumber);

    return numbers;
}

double getAverege(std::vector<int> numbers)
{
    unsigned long sum = 0;
    int quantity = 0;

    double time = omp_get_wtime();

    for (int number : numbers)
    {
        sum += number;
        quantity++;
    }

    double measuredTime = omp_get_wtime() - time;

    std::cout << "Czas liczenia sredniej arytmetycznej przygotowanego juz zbioru danych: " << measuredTime << std::endl;

    return (double)sum / (double)quantity;
}

double getAveregeWithValidationParallel(std::vector<int> numbers, int lowerLimit, int upperLimit, int minimalDigitsSum, int threads = 1)
{
    unsigned long sum = 0;
    int quantity = 0;

    double time = omp_get_wtime();
    int numbers_size = numbers.size();

#pragma omp parallel for reduction(+:sum,quantity) num_threads(threads)
    for (int index = 0; index < numbers_size; index++)
    {
        int number = numbers[index];
        if (number >= lowerLimit && number <= upperLimit)
        {
            std::string strNumber = std::to_string(number);
            int actualDigitsSum = 0;

            for (char digit : strNumber)
                actualDigitsSum += digit - '0';

            if (actualDigitsSum > minimalDigitsSum)
            {
                sum += number;
                quantity++;
            }
        }
    }
    

    double measuredTime = omp_get_wtime() - time;

    if (threads > 1)
        std::cout << "Czas liczenia sredniej arytmetycznej z walidacja danych wejsciowych przy uzyciu " << threads << " watkow wynosi: " << measuredTime << std::endl;
    else
        std::cout << "Czas liczenia sredniej arytmetycznej z walidacja danych wejsciowych przy uzyciu " << threads << " watka wynosi: " << measuredTime << std::endl;

    return (double)sum / (double)quantity;
}

int main()
{
    int lowerLimit = 1000;
    int upperLimit = 2000000;
    int minimalDigitsSum = 15;

    int minimalValueInDataset = 1;
    int maximalValueInDataset = 10000000;
    int quantityOfDataset = 5000000;

    std::cout << "Trwa przygotowywanie danych... " << std::endl;
    std::vector<int> dataset = generateRandomNumbersInRange(minimalValueInDataset, maximalValueInDataset, quantityOfDataset);
    std::cout << "Dane zostaly wygenerowane. Zaczynam obliczenia: " << std::endl << std::endl;

    std::cout << "(1) Srednia arytmetyczna z walidacja: " << getAveregeWithValidationParallel(dataset, lowerLimit, upperLimit, minimalDigitsSum) << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << "(2) (Parallel) Srednia arytmetyczna z walidacja: " << getAveregeWithValidationParallel(dataset, lowerLimit, upperLimit, minimalDigitsSum, 2) << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << "(3) (Parallel) Srednia arytmetyczna z walidacja: " << getAveregeWithValidationParallel(dataset, lowerLimit, upperLimit, minimalDigitsSum, 4) << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << "(4) (Parallel) Srednia arytmetyczna z walidacja: " << getAveregeWithValidationParallel(dataset, lowerLimit, upperLimit, minimalDigitsSum, 6) << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << "(5) (Parallel) Srednia arytmetyczna z walidacja: " << getAveregeWithValidationParallel(dataset, lowerLimit, upperLimit, minimalDigitsSum, 8) << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << "(6) (Parallel) Srednia arytmetyczna z walidacja: " << getAveregeWithValidationParallel(dataset, lowerLimit, upperLimit, minimalDigitsSum, 10) << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << "(7) (Parallel) Srednia arytmetyczna z walidacja: " << getAveregeWithValidationParallel(dataset, lowerLimit, upperLimit, minimalDigitsSum, 12) << std::endl;
}
