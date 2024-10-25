#include <mpi.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int array_size = 100;
    int local_size = array_size / size;

    std::vector<int> array;
    std::vector<int> local_array(local_size);

    // Processo raiz inicializa o array com valores aleatórios
    if (rank == 0) {
        array.resize(array_size);
        std::srand(std::time(0));
        for (int i = 0; i < array_size; ++i) {
            array[i] = std::rand() % 100;  // Valores aleatórios entre 0 e 99
        }
    }

    // Distribui o array entre os processos
    MPI_Scatter(array.data(), local_size, MPI_INT, local_array.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Calcula a média local
    double local_sum = std::accumulate(local_array.begin(), local_array.end(), 0);
    double local_mean = local_sum / local_size;

    // Calcula a variação local
    double local_variance_sum = 0.0;
    for (int value : local_array) {
        local_variance_sum += (value - local_mean) * (value - local_mean);
    }
    double local_variance = local_variance_sum / local_size;

    // Coleta as médias e variações locais no processo raiz
    std::vector<double> local_means(size);
    std::vector<double> local_variances(size);
    MPI_Gather(&local_mean, 1, MPI_DOUBLE, local_means.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Gather(&local_variance, 1, MPI_DOUBLE, local_variances.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Calcula a média global
        double global_mean = std::accumulate(local_means.begin(), local_means.end(), 0.0) / size;

        // Calcula a variância global
        double global_variance_sum = 0.0;
        for (int i = 0; i < size; ++i) {
            global_variance_sum += local_variances[i] + (local_means[i] - global_mean) * (local_means[i] - global_mean);
        }
        double global_variance = global_variance_sum / size;

        // Calcula o desvio padrão global
        double global_std_dev = std::sqrt(global_variance);
        std::cout << "Desvio padrão global do array é: " << global_std_dev << std::endl;
    }

    MPI_Finalize();
    return 0;
}
