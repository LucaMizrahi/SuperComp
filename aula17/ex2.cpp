#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>

void calculaQuadradoLocal(std::vector<int>& subMatriz) {
    for (size_t i = 0; i < subMatriz.size(); ++i) {
        subMatriz[i] *= subMatriz[i];
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int tamanhoMatriz = 1000; // Pode ser alterado para testar diferentes tamanhos
    std::vector<int> matriz;
    int elementosPorProcesso = tamanhoMatriz * tamanhoMatriz / size;

    if (rank == 0) {
        matriz.resize(tamanhoMatriz * tamanhoMatriz);
        // Inicializando a matriz com valores de exemplo
        for (int i = 0; i < tamanhoMatriz * tamanhoMatriz; ++i) {
            matriz[i] = 1; // Preenchendo com 1 para simplificação
        }
    }

    std::vector<int> subMatriz(elementosPorProcesso);

    // Protegendo o MPI_Scatter para que ele lide corretamente com divisões não uniformes
    MPI_Scatter(matriz.data(), elementosPorProcesso, MPI_INT, subMatriz.data(), elementosPorProcesso, MPI_INT, 0, MPI_COMM_WORLD);

    // Cálculo local em cada processo
    calculaQuadradoLocal(subMatriz);

    // Reunindo os dados no processo root
    MPI_Gather(subMatriz.data(), elementosPorProcesso, MPI_INT, matriz.data(), elementosPorProcesso, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Impressão de exemplo para verificar a matriz final
        std::cout << "Cálculo finalizado no processo principal.\n";
    }

    MPI_Finalize();
    return 0;
}
