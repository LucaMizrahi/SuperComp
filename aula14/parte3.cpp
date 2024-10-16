#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include <omp.h>

int main() {
    int N = 100000;  // Número de pontos a serem gerados
    int pontos_dentro_circulo = 0;

    // Medição de tempo
    double start_time = omp_get_wtime();

    // Paralelização do loop com redução
    #pragma omp parallel for reduction(+:pontos_dentro_circulo)
    for (int i = 0; i < N; ++i) {
        // Cada thread tem seu próprio gerador de números aleatórios
        std::random_device rd;  // Gerador de seed
        std::mt19937 gen(rd() + omp_get_thread_num());  // Mersenne Twister com seed único para cada thread
        std::uniform_real_distribution<> dis(-1.0, 1.0);  // Distribuição uniforme entre -1 e 1

        // Gera dois números aleatórios entre -1 e 1
        double x = dis(gen);
        double y = dis(gen);

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
    std::cout << "Estimativa de Pi (melhorado): " << pi_estimate << std::endl;
    std::cout << "Tempo de execução (melhorado): " << tempo_execucao << " segundos" << std::endl;

    return 0;
}
