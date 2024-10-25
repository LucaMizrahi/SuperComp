#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#ifdef _OPENMP
#include <omp.h>
#endif

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int array_size = 100;  // Tamanho do array
    int local_size = array_size / size;  // Tamanho da parte local de cada processo
    std::vector<int> array;  // Array principal (somente no processo raiz)
    int max_val;

    // Processo raiz inicializa o array com valores aleatórios e calcula o máximo
    if (rank == 0) {
        array.resize(array_size);
        std::srand(std::time(0));
        for (int i = 0; i < array_size; ++i) {
            array[i] = std::rand() % 100 + 1;  // Valores aleatórios entre 1 e 100
        }

        // Encontrar o valor máximo usando OpenMP (opcional)
        #ifdef _OPENMP
        max_val = array[0];
        #pragma omp parallel for reduction(max: max_val)
        #endif
        for (int i = 0; i < array_size; ++i) {
            if (array[i] > max_val) {
                max_val = array[i];
            }
        }
        std::cout << "Processo raiz encontrou o valor máximo: " << max_val << std::endl;
    }

    // Transmite o valor máximo para todos os processos
    MPI_Bcast(&max_val, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Divide o array entre os processos
    std::vector<int> local_array(local_size);
    MPI_Scatter(array.data(), local_size, MPI_INT, local_array.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo normaliza sua parte do array
    std::vector<double> local_normalized(local_size);
    for (int i = 0; i < local_size; ++i) {
        local_normalized[i] = static_cast<double>(local_array[i]) / max_val;
    }

    // Coleta os arrays normalizados no processo raiz
    std::vector<double> normalized_array;
    if (rank == 0) {
        normalized_array.resize(array_size);
    }
    MPI_Gather(local_normalized.data(), local_size, MPI_DOUBLE, normalized_array.data(), local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Processo raiz imprime o array normalizado
    if (rank == 0) {
        std::cout << "Array normalizado: ";
        for (double val : normalized_array) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
