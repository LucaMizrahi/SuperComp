#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <omp.h>

// Função para gerar um número aleatório entre 0 e 1
double random_double() {
    return static_cast<double>(rand()) / RAND_MAX;
}

int main() {
    int N = 100000;  // Número de pontos a serem gerados
    int pontos_dentro_circulo = 0;

    // Inicializa a semente globalmente para todos os threads
    std::srand(std::time(nullptr));

    // Medição de tempo
    double start_time = omp_get_wtime();

    // Paralelização do loop com redução
    #pragma omp parallel for reduction(+:pontos_dentro_circulo)
    for (int i = 0; i < N; ++i) {
        // Gera dois números aleatórios entre -1 e 1
        double x = random_double() * 2.0 - 1.0;
        double y = random_double() * 2.0 - 1.0;

        // Verifica se o ponto (x, y) está dentro do círculo
        if (x * x + y * y <= 1.0) {
            ++pontos_dentro_circulo;
        }
    }

    // Estimação de pi
    double pi_estimate = 4.0 * pontos_dentro_circulo / N;

    // Medição de tempo
    double end_time = omp_get_wtime();
    double tempo_execucao = end_time - start_time;

    // Exibe o resultado
    std::cout << "Estimativa de Pi (paralelo): " << pi_estimate << std::endl;
    std::cout << "Tempo de execução (paralelo): " << tempo_execucao << " segundos" << std::endl;

    return 0;
}
