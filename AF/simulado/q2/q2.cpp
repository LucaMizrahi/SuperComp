#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

int main(int argc, char** argv) {
    int rank, size;

    // Inicializa MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Defina o tamanho do vetor e inicialize no processo raiz
    const int VECTOR_SIZE = 10; // Você pode alterar para qualquer tamanho
    std::vector<int> vector;
    if (rank == 0) {
        vector.resize(VECTOR_SIZE);
        for (int i = 0; i < VECTOR_SIZE; ++i) {
            vector[i] = i + 1; // Inicializa com valores de 1 a VECTOR_SIZE
        }
    }

    // Calcula o tamanho do subvetor para cada processo
    int base_chunk_size = VECTOR_SIZE / size; // Parte básica para todos os processos
    int remainder = VECTOR_SIZE % size;      // Resto para redistribuição
    std::vector<int> send_counts(size), displs(size);

    for (int i = 0; i < size; ++i) {
        send_counts[i] = base_chunk_size + (i < remainder ? 1 : 0); // Distribui o resto
        displs[i] = (i == 0 ? 0 : displs[i - 1] + send_counts[i - 1]); // Deslocamento
    }

    // Aloca subvetor para cada processo
    std::vector<int> sub_vector(send_counts[rank]);

    // Distribui o vetor do processo raiz para todos os processos
    MPI_Scatterv(
        vector.data(), send_counts.data(), displs.data(), MPI_INT, 
        sub_vector.data(), send_counts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo calcula a soma dos quadrados do seu subvetor usando OpenMP
    double local_sum = 0.0;
    #pragma omp parallel for reduction(+:local_sum)
    for (int i = 0; i < sub_vector.size(); ++i) {
        local_sum += std::pow(sub_vector[i], 2);
    }

    // Reúne os resultados parciais no processo raiz
    double total_sum = 0.0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Exibe os resultados parciais em cada processo
    std::vector<double> partial_sums(size);
    MPI_Gather(&local_sum, 1, MPI_DOUBLE, partial_sums.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Processo raiz exibe o resultado final
    if (rank == 0) {
        std::cout << "Vetor utilizado: ";
        for (int val : vector) {
            std::cout << val << " ";
        }
        std::cout << "\n";

        for (int i = 0; i < size; ++i) {
            std::cout << "Processo " << i << " recebeu: ";
            for (int j = displs[i]; j < displs[i] + send_counts[i]; ++j) {
                std::cout << vector[j] << " ";
            }
            std::cout << "e calculou soma dos quadrados = " << partial_sums[i] << "\n";
        }

        std::cout << "Soma total dos quadrados: " << total_sum << "\n";
    }

    // Finaliza MPI
    MPI_Finalize();
    return 0;
}
