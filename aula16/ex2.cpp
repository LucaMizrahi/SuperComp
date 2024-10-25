#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Obter o rank do processo
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Obter o número total de processos

    int num_iteracoes = 1000;  // Número de iterações padrão

    if (rank == 0) {
        // Processo raiz define o número de iterações
        num_iteracoes = 5000;  // Pode ser modificado para qualquer valor desejado
        std::cout << "Processo raiz definiu o número de iterações como " << num_iteracoes << std::endl;
    }

    // Distribui a configuração para todos os processos
    MPI_Bcast(&num_iteracoes, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo realiza o cálculo com o número de iterações especificado
    double resultado_local = 0.0;
    for (int i = 0; i < num_iteracoes; ++i) {
        resultado_local += std::sin(i * 0.001);  // Cálculo de exemplo
    }

    // Cada processo imprime seu resultado local
    std::cout << "Processo " << rank << " finalizou o cálculo com resultado: " << resultado_local << std::endl;

    MPI_Finalize();
    return 0;
}
