#include <iostream>
#include <vector>
#include <omp.h>

// Função que calcula a soma dos elementos de um vetor
double vectorSum(const std::vector<double>& vec) {
    double sum = 0.0;

    // Paraleliza a soma dos elementos do vetor
    #pragma omp parallel for reduction(+:sum)
    for (size_t i = 0; i < vec.size(); ++i) {
        sum += vec[i];
    }

    return sum;
}

int main() {
    // Lista de vetores para processamento
    std::vector<std::vector<double>> listOfVectors = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0}
    };

    // Paraleliza a iteração pelos vetores
    #pragma omp parallel for
    for (size_t i = 0; i < listOfVectors.size(); ++i) {
        double sum = vectorSum(listOfVectors[i]);

        // Seção crítica para evitar conflitos de impressão
        #pragma omp critical
        {
            std::cout << "Vector: ";
            for (const auto& val : listOfVectors[i]) {
                std::cout << val << " ";
            }
            std::cout << "-> Sum: " << sum << std::endl;
        }
    }

    return 0;
}
