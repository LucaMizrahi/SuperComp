#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

int main() {
    // Inicializa o gerador de números aleatórios com base no tempo
    srand(static_cast<unsigned int>(time(0)));

    int N = 100000;  // Número de pontos a serem gerados
    int pontos_dentro_circulo = 0;

    // Medição de tempo
    clock_t start = clock();

    // Gera N pontos aleatórios e verifica quantos estão dentro do círculo
    for (int i = 0; i < N; ++i) {
        // Gera dois números aleatórios entre -1 e 1
        double x = static_cast<double>(rand()) / RAND_MAX * 2.0 - 1.0;
        double y = static_cast<double>(rand()) / RAND_MAX * 2.0 - 1.0;

        // Verifica se o ponto (x, y) está dentro do círculo
        if (x * x + y * y <= 1.0) {
            ++pontos_dentro_circulo;
        }
    }

    // Estimação de pi
    double pi_estimate = 4.0 * pontos_dentro_circulo / N;

    // Medição de tempo
    clock_t end = clock();
    double tempo_execucao = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    // Exibe o resultado
    std::cout << "Estimativa de Pi: " << pi_estimate << std::endl;
    std::cout << "Tempo de execução: " << tempo_execucao << " segundos" << std::endl;

    return 0;
}
