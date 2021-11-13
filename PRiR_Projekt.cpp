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
    //double time = omp_get_wtime();

    for (int i = lowerLimit; i <= upperLimit; i++)
    {
        std::string number = std::to_string(i);
        int actualDigitsSum = 0;

        for (char digit : number)
            actualDigitsSum += digit - '0';

        if (actualDigitsSum > minimalDigitsSum)
            selectedNumbers.push_back(i);
    }

    //double measuredTime = omp_get_wtime() - time;
    //std::cout << "Czas tworzenia zbioru danych: " << measuredTime << std::endl;

    return selectedNumbers;
}

std::vector<int> inRange(int lowerLimit, int upperLimit)
{
    std::vector<int> numbers;

    for (int i = lowerLimit; i <= upperLimit; i++)
        numbers.push_back(i);

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

double getAveregeWithValidation(std::vector<int> numbers, int lowerLimit, int upperLimit, int minimalDigitsSum)
{
    unsigned long sum = 0;
    int quantity = 0;

    double time = omp_get_wtime();

    for (int number : numbers)
    {
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

    std::cout << "Czas liczenia sredniej arytmetycznej z walidacja danych wejsciowych: " << measuredTime << std::endl;

    return (double)sum / (double)quantity;
}

double getAveregeWithValidationParallel(std::vector<int> numbers, int lowerLimit, int upperLimit, int minimalDigitsSum)
{
    unsigned long sum = 0;
    int quantity = 0;

    double time = omp_get_wtime();
    int numbers_size = numbers.size();

    // Ta petla rownolegla wcale nie wykonuje sie szybciej wiec to jest jeszcze do poprawy
    #pragma omp parallel num_threads(8)
    {
        #pragma omp for
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
    }
    

    double measuredTime = omp_get_wtime() - time;

    std::cout << "Czas liczenia sredniej arytmetycznej z walidacja danych wejsciowych: " << measuredTime << std::endl;

    return (double)sum / (double)quantity;
}

int main()
{
    int lowerLimit = 1000;
    int upperLimit = 2000000;
    int minimalDigitsSum = 15;

    std::cout << "Trwa przygotowywanie danych... " << std::endl;
    std::vector<int> dataset1 = getSelectedNumbers(lowerLimit, upperLimit, minimalDigitsSum);
    std::vector<int> dataset2 = inRange(lowerLimit, upperLimit);
    std::cout << "Dane zostaly wygenerowane. Zaczynam obliczenia: " << std::endl << std::endl;

    std::cout << "(1) Srednia arytmetyczna bez walidacji: " << getAverege(dataset1) << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << "(2) Srednia arytmetyczna z walidacja: " << getAveregeWithValidation(dataset2, lowerLimit, upperLimit, minimalDigitsSum) << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << "(3) (Parallel) Srednia arytmetyczna z walidacja: " << getAveregeWithValidation(dataset2, lowerLimit, upperLimit, minimalDigitsSum) << std::endl;
}
