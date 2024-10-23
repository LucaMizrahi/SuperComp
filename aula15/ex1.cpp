#include <mpi.h>
#include <iostream>
#include <cstring>

int main(int argc, char** argv) {
    // Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);

    // Determina o rank (ID) do processo atual
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        // Processo de rank 0 envia "Olá" para o processo de rank 1
        const char mensagem[] = "Olá";
        MPI_Send(mensagem, strlen(mensagem) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        std::cout << "Processo 0 enviou: " << mensagem << std::endl;

        // Processo 0 recebe a resposta "Oi" do processo de rank 1
        char resposta[10];
        MPI_Recv(resposta, 10, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo 0 recebeu: " << resposta << std::endl;

    } else if (rank == 1) {
        // Processo de rank 1 recebe a mensagem "Olá" do processo de rank 0
        char mensagem[10];
        MPI_Recv(mensagem, 10, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo 1 recebeu: " << mensagem << std::endl;

        // Processo de rank 1 responde com "Oi" para o processo de rank 0
        const char resposta[] = "Oi";
        MPI_Send(resposta, strlen(resposta) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        std::cout << "Processo 1 enviou: " << resposta << std::endl;
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();
    return 0;
}
