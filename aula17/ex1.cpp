#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

void calculaQuadradoComOpenMP(std::vector<std::vector<int>>& matriz) {
    int linhas = matriz.size();
    int colunas = matriz[0].size();

    #pragma omp parallel for
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            matriz[i][j] *= matriz[i][j];
        }
    }
}

void calculaQuadradoSemOpenMP(std::vector<std::vector<int>>& matriz) {
    int linhas = matriz.size();
    int colunas = matriz[0].size();

    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            matriz[i][j] *= matriz[i][j];
        }
    }
}

int main() {
    int tamanho = 20000; // Pode variar para testar diferentes tamanhos
    std::vector<std::vector<int>> matriz(tamanho, std::vector<int>(tamanho, 1)); // Inicializa com 1 para simplificação

    // Medindo tempo com OpenMP
    auto inicioComOMP = std::chrono::high_resolution_clock::now();
    calculaQuadradoComOpenMP(matriz);
    auto fimComOMP = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracaoComOMP = fimComOMP - inicioComOMP;
    std::cout << "Tempo com OpenMP: " << duracaoComOMP.count() << " segundos" << std::endl;

    // Resetando a matriz
    std::fill(matriz.begin(), matriz.end(), std::vector<int>(tamanho, 1));

    // Medindo tempo sem OpenMP
    auto inicioSemOMP = std::chrono::high_resolution_clock::now();
    calculaQuadradoSemOpenMP(matriz);
    auto fimSemOMP = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracaoSemOMP = fimSemOMP - inicioSemOMP;
    std::cout << "Tempo sem OpenMP: " << duracaoSemOMP.count() << " segundos" << std::endl;

    return 0;
}
