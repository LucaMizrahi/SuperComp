#include <iostream>
#include <vector>
#include <mpi.h>
#include <omp.h>
#include <numeric>
#include <cstdlib>

// Tamanho do vetor de teste reduzido para facilitar a visualização
#define MATRIX_SIZE 4

// Função para calcular a média das linhas de uma submatriz
std::vector<double> calculateLineAverages(const std::vector<std::vector<int>>& submatrix) {
    std::vector<double> averages(submatrix.size());

    #pragma omp parallel for
    for (size_t i = 0; i < submatrix.size(); ++i) {
        double sum = std::accumulate(submatrix[i].begin(), submatrix[i].end(), 0.0);
        averages[i] = sum / submatrix[i].size();
    }

    return averages;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Tamanho do chunk (número de linhas por processo)
    int chunk_size = MATRIX_SIZE / size;
    int remainder = MATRIX_SIZE % size;

    // Processo raiz inicializa a matriz
    std::vector<std::vector<int>> matrix;
    if (rank == 0) {
        matrix.resize(MATRIX_SIZE, std::vector<int>(MATRIX_SIZE));
        for (int i = 0; i < MATRIX_SIZE; ++i) {
            for (int j = 0; j < MATRIX_SIZE; ++j) {
                matrix[i][j] = (i + 1) * (j + 1); // Valores fixos para facilitar a verificação
            }
        }

        // Exibe a matriz de entrada
        std::cout << "Matriz de entrada:" << std::endl;
        for (const auto& row : matrix) {
            for (const auto& val : row) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    }

    // Determina o número de linhas que cada processo receberá
    int start_line = rank * chunk_size + std::min(rank, remainder);
    int end_line = start_line + chunk_size + (rank < remainder ? 1 : 0);

    // Envia as submatrizes para os processos
    int num_lines = end_line - start_line;
    std::vector<std::vector<int>> submatrix(num_lines, std::vector<int>(MATRIX_SIZE));
    if (rank == 0) {
        for (int i = 0; i < size; ++i) {
            int proc_start = i * chunk_size + std::min(i, remainder);
            int proc_end = proc_start + chunk_size + (i < remainder ? 1 : 0);

            if (i == 0) {
                submatrix.assign(matrix.begin() + proc_start, matrix.begin() + proc_end);
            } else {
                for (int line = proc_start; line < proc_end; ++line) {
                    MPI_Send(matrix[line].data(), MATRIX_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
            }
        }
    } else {
        for (int i = 0; i < num_lines; ++i) {
            MPI_Recv(submatrix[i].data(), MATRIX_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    // Calcula as médias das linhas locais
    std::vector<double> local_averages = calculateLineAverages(submatrix);

    // Coleta as médias parciais no processo raiz
    std::vector<double> all_averages;
    if (rank == 0) {
        all_averages.resize(MATRIX_SIZE);
    }

    std::vector<int> counts(size);
    std::vector<int> displs(size);
    for (int i = 0; i < size; ++i) {
        counts[i] = (i < remainder ? chunk_size + 1 : chunk_size);
        displs[i] = i * chunk_size + std::min(i, remainder);
    }

    MPI_Gatherv(local_averages.data(), local_averages.size(), MPI_DOUBLE,
                all_averages.data(), counts.data(), displs.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Calcula a média total e exibe os resultados no processo raiz
    if (rank == 0) {
        double total_average = std::accumulate(all_averages.begin(), all_averages.end(), 0.0) / MATRIX_SIZE;
        std::cout << "\nMédias das linhas:" << std::endl;
        for (size_t i = 0; i < all_averages.size(); ++i) {
            std::cout << "Linha " << i << ": " << all_averages[i] << std::endl;
        }
        std::cout << "\nMédia total de todas as linhas: " << total_average << std::endl;
    }

    MPI_Finalize();
    return 0;
}
