#include <iostream>
#include <vector>
#include <chrono>

int main() {
    int N = 1000;  // Define o tamanho da matriz como N x N.

    // Medição do tempo de alocação, soma e liberação de memória usando std::vector.
    auto inicio_vector = std::chrono::high_resolution_clock::now();

    // Alocação da matriz usando std::vector.
    std::vector<std::vector<int>> matriz(N, std::vector<int>(N, 0));

    // Inicialização e soma dos elementos da matriz.
    long long soma_vector = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matriz[i][j] = i + j;  // Exemplo de inicialização
            soma_vector += matriz[i][j];
        }
    }

    auto fim_vector = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao_vector = fim_vector - inicio_vector;
    std::cout << "Tempo usando std::vector: " << duracao_vector.count() << " segundos\n";
    std::cout << "Soma (vector): " << soma_vector << "\n";
    
    return 0;
}
