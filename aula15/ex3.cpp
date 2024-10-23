#include <mpi.h>
#include <iostream>
#include <cstring>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // Inicializa o ambiente MPI

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obter o rank do processo atual
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obter o número total de processos

    int mensagem;

    if (rank == 0) {
        // Processo 0 inicia a mensagem
        mensagem = 100;  // Mensagem inicial
        std::cout << "Processo 0 iniciou a mensagem com valor " << mensagem << std::endl;
        // Envia a mensagem para o processo 1
        MPI_Send(&mensagem, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else {
        // Todos os outros processos recebem do processo anterior
        MPI_Recv(&mensagem, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu a mensagem com valor " << mensagem << " do processo " << rank - 1 << std::endl;

        // Envia a mensagem para o próximo processo, exceto o último
        if (rank < size - 1) {
            MPI_Send(&mensagem, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize(); // Finaliza o ambiente MPI
    return 0;
}
