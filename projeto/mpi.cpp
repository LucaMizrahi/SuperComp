#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <mpi.h>
#include <chrono>
#include <functional>

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

void EncontrarCliqueLocal(const std::vector<std::vector<int>>& grafo, std::vector<int>& cliqueMaxima, int numVertices, int rank, int size) {
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

    for (int i = rank; i < numVertices; i += size) {
        cliqueAtual.clear();
        cliqueAtual.push_back(i);
        std::vector<int> candidatos;
        for (int j = i + 1; j < numVertices; ++j) {
            if (grafo[i][j]) {
                candidatos.push_back(j);
            }
        }
        EncontrarCliques(cliqueAtual, candidatos);
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int numVertices;
    std::string nomeArquivo = "grafos/grafo100.txt";
    std::vector<std::vector<int>> grafo;

    if (rank == 0) {
        grafo = LerGrafo(nomeArquivo, numVertices);
    }

    MPI_Bcast(&numVertices, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        grafo.resize(numVertices, std::vector<int>(numVertices, 0));
    }

    for (int i = 0; i < numVertices; ++i) {
        MPI_Bcast(grafo[i].data(), numVertices, MPI_INT, 0, MPI_COMM_WORLD);
    }

    std::vector<int> cliqueMaximaLocal;
    auto start = std::chrono::high_resolution_clock::now();
    EncontrarCliqueLocal(grafo, cliqueMaximaLocal, numVertices, rank, size);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    int tamanhoMaximoLocal = cliqueMaximaLocal.size();
    int tamanhoMaximoGlobal;
    MPI_Reduce(&tamanhoMaximoLocal, &tamanhoMaximoGlobal, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (tamanhoMaximoLocal == tamanhoMaximoGlobal) {
        MPI_Send(cliqueMaximaLocal.data(), tamanhoMaximoLocal, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        std::vector<int> cliqueMaximaGlobal(tamanhoMaximoGlobal);
        MPI_Recv(cliqueMaximaGlobal.data(), tamanhoMaximoGlobal, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::cout << "Tamanho da clique máxima: " << cliqueMaximaGlobal.size() << std::endl;
        std::cout << "Vértices da clique máxima: ";
        for (int v : cliqueMaximaGlobal) {
            std::cout << (v + 1) << " ";
        }
        std::cout << std::endl;
        std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
