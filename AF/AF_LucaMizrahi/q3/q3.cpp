#include <iostream>
#include <vector>
#include <omp.h>
#include <limits>

// Função para calcular a média
double calculateMean(const std::vector<int>& vec) {
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum)
    for (size_t i = 0; i < vec.size(); ++i) {
        sum += vec[i];
    }

    return sum / vec.size();
}

// Função para encontrar o maior valor
int calculateMax(const std::vector<int>& vec) {
    int maxVal = std::numeric_limits<int>::min();

    #pragma omp parallel for reduction(max:maxVal)
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] > maxVal) {
            maxVal = vec[i];
        }
    }

    return maxVal;
}

// Função para calcular o produto dos elementos
long long calculateProduct(const std::vector<int>& vec) {
    long long product = 1;

    #pragma omp parallel for reduction(*:product)
    for (size_t i = 0; i < vec.size(); ++i) {
        product *= vec[i];
    }

    return product;
}

int main() {
    // Vetor de exemplo para cálculo
    std::vector<int> vec = {1, 2, 3, 4, 5};

    // Paraleliza a execução das três tarefas simultaneamente
    double mean;
    int maxVal;
    long long product;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            mean = calculateMean(vec);
        }
        #pragma omp section
        {
            maxVal = calculateMax(vec);
        }
        #pragma omp section
        {
            product = calculateProduct(vec);
        }
    }

    // Exibe os resultados
    std::cout << "Vetor: ";
    for (const auto& num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    std::cout << "Média: " << mean << std::endl;
    std::cout << "Maior valor: " << maxVal << std::endl;
    std::cout << "Produto dos elementos: " << product << std::endl;

    return 0;
}
