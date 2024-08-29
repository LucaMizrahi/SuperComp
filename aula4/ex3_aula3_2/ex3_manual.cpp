#include <iostream>
#include <chrono>

int main() {
    int N = 1000;  // Define o tamanho da matriz como N x N.

    // Medição do tempo de alocação, soma e liberação de memória usando alocação dinâmica manual.
    auto inicio_manual = std::chrono::high_resolution_clock::now();

    // Alocação dinâmica de memória para uma matriz N x N.
    int** matriz = new int*[N];
    for (int i = 0; i < N; ++i) {
        matriz[i] = new int[N];
    }

    // Inicialização e soma dos elementos da matriz.
    long long soma_manual = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matriz[i][j] = i + j;  // Exemplo de inicialização
            soma_manual += matriz[i][j];
        }
    }

    // Liberação da memória alocada dinamicamente.
    for (int i = 0; i < N; ++i) {
        delete[] matriz[i];
    }
    delete[] matriz;

    auto fim_manual = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao_manual = fim_manual - inicio_manual;
    std::cout << "Tempo usando alocação dinâmica manual: " << duracao_manual.count() << " segundos\n";
    std::cout << "Soma (manual): " << soma_manual << "\n";
    
    return 0;
}
