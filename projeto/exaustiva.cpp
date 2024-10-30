#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <chrono>

// Função para ler o grafo a partir do arquivo de entrada
std::vector<std::vector<int>> LerGrafo(const std::string& nomeArquivo, int& numVertices) {
    std::ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;

    // Inicializa a matriz de adjacência com zeros
    std::vector<std::vector<int>> grafo(numVertices, std::vector<int>(numVertices, 0));

    // Lê as arestas e preenche a matriz de adjacência
    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;  // O grafo é não direcionado
    }

    arquivo.close();
    return grafo;
}

// Função para encontrar a clique máxima usando abordagem exaustiva
void EncontrarCliqueMaxima(const std::vector<std::vector<int>>& grafo, int numVertices, std::vector<int>& cliqueMaxima) {
    // Inicializa os candidatos com todos os vértices do grafo
    std::vector<int> candidatos(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        candidatos[i] = i;
    }

    // Vetor auxiliar para a clique atual
    std::vector<int> cliqueAtual;

    // Função lambda recursiva para encontrar cliques
    std::function<void(std::vector<int>&, std::vector<int>&)> EncontrarCliques;
    EncontrarCliques = [&](std::vector<int>& cliqueAtual, std::vector<int>& candidatos) {
        for (size_t i = 0; i < candidatos.size(); ++i) {
            int v = candidatos[i];
            std::vector<int> novosCandidatos;

            // Filtra os candidatos para aqueles que são adjacentes ao vértice atual v
            for (size_t j = i + 1; j < candidatos.size(); ++j) {
                int u = candidatos[j];
                if (grafo[v][u]) {
                    novosCandidatos.push_back(u);
                }
            }

            // Adiciona o vértice atual à clique
            cliqueAtual.push_back(v);

            // Atualiza a clique máxima se a clique atual for maior
            if (cliqueAtual.size() > cliqueMaxima.size()) {
                cliqueMaxima = cliqueAtual;
            }

            // Chama recursivamente para explorar com os novos candidatos
            if (!novosCandidatos.empty()) {
                EncontrarCliques(cliqueAtual, novosCandidatos);
            }

            // Remove o vértice da clique atual ao voltar na recursão
            cliqueAtual.pop_back();
        }
    };

    // Inicia a busca pela clique máxima
    EncontrarCliques(cliqueAtual, candidatos);
}

int main() {
    int numVertices;
    std::string nomeArquivo = "grafo50.txt";
    
    // Lê o grafo do arquivo e armazena como matriz de adjacência
    std::vector<std::vector<int>> grafo = LerGrafo(nomeArquivo, numVertices);

    // Vetor para armazenar a clique máxima encontrada
    std::vector<int> cliqueMaxima;
    
    // Encontra a clique máxima
    EncontrarCliqueMaxima(grafo, numVertices, cliqueMaxima);

    // Início da medição de tempo
    auto start = std::chrono::high_resolution_clock::now();
    EncontrarCliqueMaxima(grafo, numVertices, cliqueMaxima);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Exibe o resultado
    std::cout << "Tamanho da clique máxima: " << cliqueMaxima.size() << std::endl;
    std::cout << "Vértices da clique máxima: ";
    for (int v : cliqueMaxima) {
        std::cout << (v + 1) << " "; // Exibindo os vértices na notação 1-baseada
    }
    std::cout << std::endl;
    std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;

    return 0;
}
