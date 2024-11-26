#include <iostream>
#include <omp.h>

long long factorial(int n) {
    long long result = 1;

    // Paralelizando o cálculo do fatorial
    #pragma omp parallel for reduction(*:result)
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }

    return result;
}

int main() {
    int number = 5;

    // Exibe o número de threads disponíveis
    #pragma omp parallel
    {
        #pragma omp single
        std::cout << "Número de threads disponíveis: " << omp_get_num_threads() << std::endl;
    }

    // Calcula o fatorial usando a função paralelizada
    std::cout << "Fatorial de " << number << " é " << factorial(number) << std::endl;

    return 0;
}
