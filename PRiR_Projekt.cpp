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

std::vector<int> getSelectedNumbers(int lowerLimit, int upperLimit, int minimalDigitsSum)
{
    std::vector<int> selectedNumbers;
    double time = omp_get_wtime();

    for (int i = lowerLimit; i <= upperLimit; i++)
    {
        std::string number = std::to_string(i);
        int actualDigitsSum = 0;

        for (char digit : number)
            actualDigitsSum += digit - '0';

        if (actualDigitsSum > minimalDigitsSum)
            selectedNumbers.push_back(i);
    }
    double measuredTime = omp_get_wtime() - time;
    std::cout << "Czas tworzenia zbioru danych: " << measuredTime << std::endl;
    return selectedNumbers;
}

double getAverege(std::vector<int> numbers)
{
    int sum = 0;
    int quantity = 0;

    double time = omp_get_wtime();

    for (int number : numbers)
    {
        sum += number;
        quantity++;
    }

    double measuredTime = omp_get_wtime() - time;

    std::cout << "Czas liczenia sredniej arytmetycznej: " << measuredTime << std::endl;

    return (double)sum / (double)quantity;
}

int main()
{
    int lowerLimit = 10000;
    int upperLimit = 100000;
    int minimalDigitsSum = 15;

    std::vector<int> dataset = getSelectedNumbers(lowerLimit, upperLimit, minimalDigitsSum);

    std::cout << "Srednia arytmetyczna zadanego zbioru danych wynosi: " << getAverege(dataset) << std::endl;
}
