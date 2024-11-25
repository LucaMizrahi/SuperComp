#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>

#define VECTOR_SIZE 5

int main(int argc, char *argv[]) {
    int rank, size, i;
    double local_sum = 0.0, total_sum = 0.0;
    int chunk_size, remainder;

    // Inicializa o MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtém o rank do processo
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtém o número total de processos

    // Calcula o tamanho do subvetor que cada processo irá manipular
    chunk_size = VECTOR_SIZE / size;
    remainder = VECTOR_SIZE % size;
    std::vector<double> sub_vector(chunk_size + (rank < remainder ? 1 : 0));

    std::vector<double> vector;
    if (rank == 0) {
        // Apenas o processo raiz inicializa o vetor completo
        vector.resize(VECTOR_SIZE);
        for (i = 0; i < VECTOR_SIZE; i++) {
            vector[i] = i + 1; // Inicializa o vetor com valores de 1 a VECTOR_SIZE
        }

        // Imprime o vetor completo
        std::cout << "Vector: ";
        for (i = 0; i < VECTOR_SIZE; i++) {
            std::cout << vector[i] << " ";
        }
        std::cout << std::endl;
    }

    // Distribui partes do vetor completo para todos os processos
    std::vector<int> sendcounts(size);
    std::vector<int> displs(size);
    int offset = 0;
    for (i = 0; i < size; i++) {
        sendcounts[i] = chunk_size + (i < remainder ? 1 : 0);
        displs[i] = offset;
        offset += sendcounts[i];
    }

    MPI_Scatterv(vector.data(), sendcounts.data(), displs.data(), MPI_DOUBLE, sub_vector.data(), sub_vector.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Calcula a soma dos quadrados dos elementos do subvetor usando OpenMP
    #pragma omp parallel for reduction(+:local_sum)
    for (i = 0; i < sub_vector.size(); i++) {
        local_sum += sub_vector[i] * sub_vector[i];
    }

    // Imprime o resultado parcial de cada processo
    std::cout << "Process " << rank << " partial sum of squares: " << local_sum << std::endl;

    // Reduz os resultados parciais de todos os processos para o processo raiz
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // O processo raiz imprime o resultado final
    if (rank == 0) {
        std::cout << "Total sum of squares: " << total_sum << std::endl;
    }

    // Finaliza o MPI
    MPI_Finalize();
    return 0;
}