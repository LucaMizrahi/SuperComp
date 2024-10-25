#include <mpi.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Obter o rank do processo
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Obter o número total de processos

    const int array_size = 100;  // Tamanho do array principal
    int local_size = array_size / size;  // Tamanho da parte local de cada processo

    std::vector<int> array;
    std::vector<int> local_array(local_size);  // Array para a parte local de cada processo

    if (rank == 0) {
        // Processo raiz inicializa o array com valores aleatórios
        array.resize(array_size);
        std::srand(std::time(0));
        for (int i = 0; i < array_size; ++i) {
            array[i] = std::rand() % 100;  // Valores aleatórios entre 0 e 99
        }
    }

    // Distribui o array para todos os processos
    MPI_Scatter(array.data(), local_size, MPI_INT, local_array.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Calcula a média local
    int local_sum = std::accumulate(local_array.begin(), local_array.end(), 0);
    double local_mean = static_cast<double>(local_sum) / local_size;

    // Coleta as médias locais no processo raiz
    std::vector<double> local_means(size);
    MPI_Gather(&local_mean, 1, MPI_DOUBLE, local_means.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Processo raiz calcula a média global
        double global_sum = std::accumulate(local_means.begin(), local_means.end(), 0.0);
        double global_mean = global_sum / size;
        std::cout << "A média global do array é: " << global_mean << std::endl;
    }

    MPI_Finalize();
    return 0;
}
