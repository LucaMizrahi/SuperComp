#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <functional>
#include <chrono>

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

void EncontrarCliqueMaximaOpenMP(const std::vector<std::vector<int>>& grafo, int numVertices, std::vector<int>& cliqueMaxima) {
    std::vector<int> candidatos(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        candidatos[i] = i;
    }

    // Define cliqueMaxima como thread-safe com OpenMP
    #pragma omp parallel
    {
        std::vector<int> cliqueMaximaLocal;  // Clique máxima local para cada thread
        std::vector<int> cliqueAtual;        // Clique atual de cada thread

        // Usa std::function para permitir chamada recursiva da lambda
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

                if (cliqueAtual.size() > cliqueMaximaLocal.size()) {
                    cliqueMaximaLocal = cliqueAtual;
                }

                if (!novosCandidatos.empty()) {
                    EncontrarCliques(cliqueAtual, novosCandidatos);
                }

                cliqueAtual.pop_back();
            }
        };

        #pragma omp for schedule(dynamic)
        for (size_t i = 0; i < candidatos.size(); ++i) {
            cliqueAtual.clear();
            cliqueAtual.push_back(candidatos[i]);
            std::vector<int> novosCandidatos;
            for (size_t j = i + 1; j < candidatos.size(); ++j) {
                if (grafo[candidatos[i]][candidatos[j]]) {
                    novosCandidatos.push_back(candidatos[j]);
                }
            }
            EncontrarCliques(cliqueAtual, novosCandidatos);
        }

        // Atualiza a cliqueMaxima global com a clique máxima local de cada thread
        #pragma omp critical
        if (cliqueMaximaLocal.size() > cliqueMaxima.size()) {
            cliqueMaxima = cliqueMaximaLocal;
        }
    }
}

int main() {
    int numVertices;
    std::string nomeArquivo = "grafos/grafo175.txt";
    
    std::vector<std::vector<int>> grafo = LerGrafo(nomeArquivo, numVertices);

    std::vector<int> cliqueMaxima;

    // Início da medição de tempo
    auto start = std::chrono::high_resolution_clock::now();
    EncontrarCliqueMaximaOpenMP(grafo, numVertices, cliqueMaxima);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Exibe o resultado no console
    std::cout << "Tamanho da clique máxima: " << cliqueMaxima.size() << std::endl;
    std::cout << "Vértices da clique máxima: ";
    for (int v : cliqueMaxima) {
        std::cout << (v + 1) << " ";
    }
    std::cout << std::endl;
    std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;

    return 0;
}
