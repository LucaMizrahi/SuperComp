#include <mpi.h>
#include <iostream>
#include <cstring>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // Inicializa o ambiente MPI

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obter o rank do processo atual
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obter o número total de processos

    // Certificar-se de que o número de processos seja maior que 2
    if (size <= 2) {
        if (rank == 0) {
            std::cerr << "Erro: O número de processos deve ser maior que 2!" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    int mensagem = rank;  // A mensagem será o rank do processo

    // Se o processo não for o último, envia para o próximo processo
    if (rank < size - 1) {
        MPI_Send(&mensagem, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        std::cout << "Processo " << rank << " enviou mensagem para processo " << rank + 1 << std::endl;
    } else { 
        // O último processo envia de volta para o processo 0
        MPI_Send(&mensagem, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        std::cout << "Processo " << rank << " enviou mensagem para processo 0" << std::endl;
    }

    // Receber a mensagem do processo anterior
    if (rank == 0) {
        // Processo 0 recebe do último processo
        MPI_Recv(&mensagem, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo 0 recebeu mensagem de processo " << size - 1 << std::endl;
    } else {
        MPI_Recv(&mensagem, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu mensagem de processo " << rank - 1 << std::endl;
    }

    MPI_Finalize(); // Finaliza o ambiente MPI
    return 0;
}
