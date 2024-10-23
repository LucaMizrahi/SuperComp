#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);  // Inicializa o ambiente MPI

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Obter o rank do processo atual
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Obter o número total de processos

    int mensagem;  // Variável para enviar e receber mensagens

    if (rank == 0) {
        // Processo 0 envia e recebe mensagens alternadamente com os outros processos
        for (int i = 1; i < size; i++) {
            // Envia uma mensagem para o processo i
            mensagem = i;  // Mensagem a ser enviada
            MPI_Send(&mensagem, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            std::cout << "Processo 0 enviou a mensagem " << mensagem << " para o processo " << i << std::endl;

            // Recebe a resposta do processo i
            MPI_Recv(&mensagem, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Processo 0 recebeu a resposta " << mensagem << " do processo " << i << std::endl;
        }
    } else {
        // Todos os outros processos recebem e respondem ao processo 0
        MPI_Recv(&mensagem, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu a mensagem " << mensagem << " do processo 0" << std::endl;

        // Envia a resposta de volta para o processo 0
        MPI_Send(&mensagem, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        std::cout << "Processo " << rank << " enviou a resposta " << mensagem << " para o processo 0" << std::endl;
    }

    MPI_Finalize();  // Finaliza o ambiente MPI
    return 0;
}
