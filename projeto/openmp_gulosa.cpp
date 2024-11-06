#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <chrono>
#include <fstream>

// Função para ler o grafo de um arquivo
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

// Função para encontrar uma clique usando heurística gulosa
std::vector<int> CliqueGulosa(const std::vector<std::vector<int>>& grafo, int numVertices, int inicio) {
    std::vector<int> clique;
    std::vector<bool> adicionado(numVertices, false);

    clique.push_back(inicio);
    adicionado[inicio] = true;

    for (int v = 0; v < numVertices; ++v) {
        if (!adicionado[v]) {
            bool podeAdicionar = true;
            for (int u : clique) {
                if (grafo[u][v] == 0) {
                    podeAdicionar = false;
                    break;
                }
            }
            if (podeAdicionar) {
                clique.push_back(v);
                adicionado[v] = true;
            }
        }
    }
    return clique;
}

// Função para encontrar a clique máxima aproximada com OpenMP
std::vector<int> EncontrarCliqueGulosaOpenMP(const std::vector<std::vector<int>>& grafo, int numVertices) {
    std::vector<int> cliqueMaxima;

    #pragma omp parallel
    {
        std::vector<int> cliqueLocal;
        
        #pragma omp for
        for (int i = 0; i < numVertices; ++i) {
            std::vector<int> novaClique = CliqueGulosa(grafo, numVertices, i);

            #pragma omp critical
            if (novaClique.size() > cliqueMaxima.size()) {
                cliqueMaxima = novaClique;
            }
        }
    }
    return cliqueMaxima;
}

int main() {
    int numVertices;
    std::string nomeArquivo = "grafos/grafo50.txt";
    std::vector<std::vector<int>> grafo = LerGrafo(nomeArquivo, numVertices);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> cliqueMaxima = EncontrarCliqueGulosaOpenMP(grafo, numVertices);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Tamanho da clique máxima aproximada: " << cliqueMaxima.size() << std::endl;
    std::cout << "Vértices da clique máxima aproximada: ";
    for (int v : cliqueMaxima) {
        std::cout << (v + 1) << " ";
    }
    std::cout << std::endl;
    std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;

    return 0;
}
