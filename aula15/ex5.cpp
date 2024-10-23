#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);  // Inicializa o ambiente MPI

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Obter o rank do processo atual
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Obter o número total de processos

    // Cada processo recebe um valor exclusivo (exemplo: o próprio rank)
    int valor_local = rank + 1;  // Aqui, o valor é simplesmente rank + 1

    int soma_recebida = 0;  // Inicializa a soma recebida
    if (rank == 0) {
        // O processo 0 inicia a soma com seu valor local
        soma_recebida = valor_local;
        std::cout << "Processo 0 inicia com valor: " << soma_recebida << std::endl;

        // Envia o valor inicial para o processo 1
        MPI_Send(&soma_recebida, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else {
        // Recebe a soma do processo anterior
        MPI_Recv(&soma_recebida, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        soma_recebida += valor_local;  // Soma o valor local à soma recebida
        std::cout << "Processo " << rank << " somou seu valor: " << valor_local 
                  << " para total: " << soma_recebida << std::endl;

        // Se não for o último processo, envia a soma para o próximo processo
        if (rank < size - 1) {
            MPI_Send(&soma_recebida, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        } else {
            // O último processo imprime a soma total
            std::cout << "Processo " << rank << " exibe a soma total: " << soma_recebida << std::endl;
        }
    }

    MPI_Finalize();  // Finaliza o ambiente MPI
    return 0;
}
