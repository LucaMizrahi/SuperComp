#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <mpi.h>
#include <chrono>
#include <functional>
#include <numeric>

std::vector<std::vector<int>> LerGrafo(const std::string& nomeArquivo, int& numVertices) {
    std::ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;

    std::vector<std::vector<int>> grafo(numVertices, std::vector<int>(numVertices, 0));

    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;
    }

    arquivo.close();
    return grafo;
}

void EncontrarCliqueLocal(const std::vector<std::vector<int>>& grafo, std::vector<int>& cliqueMaxima, int inicio, int fim) {
    std::vector<int> cliqueAtual;

    std::function<void(std::vector<int>&, std::vector<int>&)> EncontrarCliques;
    EncontrarCliques = [&](std::vector<int>& cliqueAtual, std::vector<int>& candidatos) {
        for (size_t i = 0; i < candidatos.size(); ++i) {
            int v = candidatos[i];
            std::vector<int> novosCandidatos;
            for (size_t j = i + 1; j < candidatos.size(); ++j) {
                int u = candidatos[j];
                if (grafo[v][u]) {
                    novosCandidatos.push_back(u);
                }
            }

            cliqueAtual.push_back(v);

            if (cliqueAtual.size() > cliqueMaxima.size()) {
                cliqueMaxima = cliqueAtual;
            }

            if (!novosCandidatos.empty()) {
                EncontrarCliques(cliqueAtual, novosCandidatos);
            }

            cliqueAtual.pop_back();
        }
    };

    std::vector<int> candidatos;
    for (int i = inicio; i < fim; ++i) {
        candidatos.push_back(i);
    }

    EncontrarCliques(cliqueAtual, candidatos);
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int numVertices;
    std::string nomeArquivo = "grafos/grafo50.txt";
    std::vector<std::vector<int>> grafo;

    if (rank == 0) {
        grafo = LerGrafo(nomeArquivo, numVertices);
    }

    // Transmite o número de vértices para todos os processos
    MPI_Bcast(&numVertices, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Transmite a matriz de adjacência inteira
    if (rank != 0) {
        grafo.resize(numVertices, std::vector<int>(numVertices, 0));
    }

    for (int i = 0; i < numVertices; ++i) {
        MPI_Bcast(grafo[i].data(), numVertices, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Calcula o intervalo de vértices para cada processo
    int verticesPorProcesso = numVertices / size;
    int inicio = rank * verticesPorProcesso;
    int fim = (rank == size - 1) ? numVertices : inicio + verticesPorProcesso;

    // Calcula a clique máxima local
    std::vector<int> cliqueMaximaLocal;
    auto start = std::chrono::high_resolution_clock::now();
    EncontrarCliqueLocal(grafo, cliqueMaximaLocal, inicio, fim);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    int tamanhoMaximoLocal = cliqueMaximaLocal.size();
    int tamanhoMaximoGlobal;

    // Reduz para encontrar o tamanho máximo global
    MPI_Reduce(&tamanhoMaximoLocal, &tamanhoMaximoGlobal, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::vector<int> cliqueMaximaGlobal = cliqueMaximaLocal;

        // Recebe cliques de outros processos e determina a maior
        for (int i = 1; i < size; ++i) {
            std::vector<int> cliqueRecebida(tamanhoMaximoGlobal);
            MPI_Recv(cliqueRecebida.data(), cliqueRecebida.size(), MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (cliqueRecebida.size() > cliqueMaximaGlobal.size()) {
                cliqueMaximaGlobal = cliqueRecebida;
            }
        }

        // Exibe a clique máxima global
        std::cout << "Tamanho da clique máxima: " << cliqueMaximaGlobal.size() << std::endl;
        std::cout << "Vértices da clique máxima: ";
        for (int v : cliqueMaximaGlobal) {
            std::cout << (v + 1) << " ";
        }
        std::cout << std::endl;
        std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;
    } else {
        // Envia a clique máxima local para o processo principal
        MPI_Send(cliqueMaximaLocal.data(), cliqueMaximaLocal.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
