#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>
#include <algorithm> // Para std::sort

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 1000; // Tamanho do vetor
    int valorBusca = 5; // Valor a ser procurado
    std::vector<int> vetor;

    // Inicializando o vetor apenas no rank=0
    if (rank == 0) {
        vetor.resize(N);
        for (int i = 0; i < N; ++i) {
            vetor[i] = i % 10; // Exemplo: preenchendo com valores de 0 a 9
        }
    }

    // Determinando o tamanho da parte que cada processo irá receber
    int elementosPorProcesso = N / size;
    std::vector<int> subVetor(elementosPorProcesso);

    // Distribuindo o vetor para os processos
    MPI_Scatter(vetor.data(), elementosPorProcesso, MPI_INT, subVetor.data(), elementosPorProcesso, MPI_INT, 0, MPI_COMM_WORLD);

    // Vetor local para armazenar as posições encontradas
    std::vector<int> posicoesLocais;

    // Busca paralela em cada subvetor com OpenMP
    #pragma omp parallel for
    for (int i = 0; i < elementosPorProcesso; ++i) {
        if (subVetor[i] == valorBusca) {
            #pragma omp critical
            posicoesLocais.push_back(i + rank * elementosPorProcesso);
        }
    }

    // Reunindo as posições encontradas no processo rank=0
    if (rank == 0) {
        std::vector<int> posicoesGlobais;

        // Recebendo posições de outros processos
        for (int i = 1; i < size; ++i) {
            int tamanho;
            MPI_Recv(&tamanho, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (tamanho > 0) {
                std::vector<int> buffer(tamanho);
                MPI_Recv(buffer.data(), tamanho, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                posicoesGlobais.insert(posicoesGlobais.end(), buffer.begin(), buffer.end());
            }
        }

        // Adicionando as próprias posições
        posicoesGlobais.insert(posicoesGlobais.end(), posicoesLocais.begin(), posicoesLocais.end());

        // Ordenando as posições em ordem crescente
        std::sort(posicoesGlobais.begin(), posicoesGlobais.end());

        // Imprimindo as posições encontradas
        std::cout << "Posições do valor " << valorBusca << " em ordem crescente: ";
        for (int pos : posicoesGlobais) {
            std::cout << pos << " ";
        }
        std::cout << std::endl;

    } else {
        // Enviando as posições encontradas para o rank=0
        int tamanho = posicoesLocais.size();
        MPI_Send(&tamanho, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        if (tamanho > 0) {
            MPI_Send(posicoesLocais.data(), tamanho, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
