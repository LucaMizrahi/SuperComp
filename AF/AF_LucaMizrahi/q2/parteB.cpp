#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>

std::vector<double> normalize(std::vector<double>& v) {
    double sum = 0;

    // Cálculo da soma dos quadrados (paralelizado)
    #pragma omp parallel for reduction(+:sum)
    for (size_t i = 0; i < v.size(); ++i) {
        sum += v[i] * v[i];
    }

    double magnitude = sqrt(sum);

    // Normalização dos elementos do vetor (paralelizado)
    #pragma omp parallel for
    for (size_t i = 0; i < v.size(); ++i) {
        v[i] /= magnitude;
    }

    return v;
}

int main() {
    // Inicializa o vetor original
    std::vector<double> vec{1.0, 2.0, 3.0, 4.0};

    // Exibe o vetor original
    std::cout << "Vetor original: ";
    for (auto v : vec) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    // Normaliza o vetor
    vec = normalize(vec);

    // Exibe o vetor normalizado
    std::cout << "Vetor normalizado: ";
    for (auto v : vec) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    return 0;
}
