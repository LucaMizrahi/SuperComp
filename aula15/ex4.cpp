#include <mpi.h>
#include <iostream>
#include <cstring>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // Inicializa o ambiente MPI

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obter o rank do processo atual
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obter o número total de processos

    if (rank == 0) {
        // Processo 0 envia mensagens exclusivas para cada processo
        for (int i = 1; i < size; i++) {
            // Construir a mensagem para o processo i
            std::string mensagem = "Mensagem para o processo " + std::to_string(i);
            MPI_Send(mensagem.c_str(), mensagem.size() + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            std::cout << "Processo 0 enviou: \"" << mensagem << "\" para o processo " << i << std::endl;
        }
    } else {
        // Todos os outros processos recebem suas mensagens do processo 0
        char mensagem_recebida[50];  // Buffer para armazenar a mensagem recebida
        MPI_Recv(mensagem_recebida, 50, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu: " << mensagem_recebida << std::endl;
    }

    MPI_Finalize(); // Finaliza o ambiente MPI
    return 0;
}
