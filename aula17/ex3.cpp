#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>
#include <numeric>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 1000000; // Tamanho do vetor
    std::vector<int> vetor;
    int elementosPorProcesso = N / size;

    if (rank == 0) {
        // Inicializando o vetor com valores de 1 a N
        vetor.resize(N);
        for (int i = 0; i < N; ++i) {
            vetor[i] = 1; // Pode ser alterado para valores aleatórios ou outros padrões
        }
    }

    // Vetor para armazenar a parte recebida por cada processo
    std::vector<int> subVetor(elementosPorProcesso);

    // Distribuição do vetor usando MPI_Scatter
    MPI_Scatter(vetor.data(), elementosPorProcesso, MPI_INT, subVetor.data(), elementosPorProcesso, MPI_INT, 0, MPI_COMM_WORLD);

    // Cálculo da soma parcial com OpenMP
    int somaParcial = 0;
    #pragma omp parallel for reduction(+:somaParcial)
    for (int i = 0; i < elementosPorProcesso; ++i) {
        somaParcial += subVetor[i];
    }

    // Coletando as somas parciais no processo rank=0
    int somaTotal = 0;
    MPI_Reduce(&somaParcial, &somaTotal, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double media = static_cast<double>(somaTotal) / N;
        std::cout << "A média dos elementos do vetor é: " << media << std::endl;
    }

    MPI_Finalize();
    return 0;
}
