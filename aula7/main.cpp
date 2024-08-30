#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>  // Biblioteca para medir o tempo de execução
#include <random>  // Biblioteca para gerar números aleatórios
#include <ctime>   // Biblioteca para usar o time como semente para o gerador de números aleatórios

// Estrutura para representar um item
struct item {
    int id;
    double peso;
    double valor;
};

// Função recursiva para resolver o problema da mochila usando busca exaustiva
double knapsack(double W, const std::vector<item>& items, int n, double& peso_total, std::vector<int>& itens_selecionados) {
    // Caso base: sem itens ou capacidade zero
    if (n == 0 || W == 0) {
        peso_total = 0;
        return 0;
    }

    // Se o peso do item n-ésimo é maior que a capacidade W, ele não pode ser incluído
    if (items[n - 1].peso > W) {
        return knapsack(W, items, n - 1, peso_total, itens_selecionados);
    } else {
        // Variáveis para armazenar peso total dos caminhos de inclusão e exclusão
        double peso_incluido, peso_excluido;
        std::vector<int> itens_incluidos, itens_excluidos;

        // Calcular o valor se incluirmos o item
        double valor_incluido = items[n - 1].valor + knapsack(W - items[n - 1].peso, items, n - 1, peso_incluido, itens_incluidos);
        peso_incluido += items[n - 1].peso;

        // Calcular o valor se não incluirmos o item
        double valor_excluido = knapsack(W, items, n - 1, peso_excluido, itens_excluidos);

        // Retornar o maior valor entre incluir ou não incluir o item
        if (valor_incluido > valor_excluido) {
            peso_total = peso_incluido;
            itens_incluidos.push_back(n - 1);  // Adiciona o item incluído à lista
            itens_selecionados = itens_incluidos;  // Atualiza a lista de itens selecionados
            return valor_incluido;
        } else {
            peso_total = peso_excluido;
            itens_selecionados = itens_excluidos;  // Atualiza a lista de itens selecionados
            return valor_excluido;
        }
    }
}

// Heurística 1: Embaralhar e Preencher a Mochila
double heuristic_shuffle_fill(double W, std::vector<item> items, double& peso_total, std::vector<int>& itens_selecionados) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(items.begin(), items.end(), g);

    double valor_total = 0;
    peso_total = 0;
    itens_selecionados.clear();

    for (size_t i = 0; i < items.size(); ++i) {
        if (peso_total + items[i].peso <= W) {
            peso_total += items[i].peso;
            valor_total += items[i].valor;
            itens_selecionados.push_back(items[i].id);
        }
    }

    return valor_total;
}

// Heurística 2: Seleção Aleatória Baseada em Probabilidade
double heuristic_random_selection(double W, const std::vector<item>& items, double& peso_total, std::vector<int>& itens_selecionados, double threshold = 0.5) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    double valor_total = 0;
    peso_total = 0;
    itens_selecionados.clear();

    for (size_t i = 0; i < items.size(); ++i) {
        if (dis(gen) > threshold && peso_total + items[i].peso <= W) {
            peso_total += items[i].peso;
            valor_total += items[i].valor;
            itens_selecionados.push_back(items[i].id);
        }
    }

    return valor_total;
}

int main() {
    std::ifstream inputFile("entrada3.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de entrada!" << std::endl;
        return 1;
    }

    int N;  // Número de itens
    double W;  // Capacidade da mochila
    inputFile >> N >> W;

    std::vector<item> items(N);
    for (int i = 0; i < N; ++i) {
        items[i].id = i + 1;
        inputFile >> items[i].peso >> items[i].valor;
    }

    inputFile.close();

    // Busca exaustiva
    double peso_total_exaustiva = 0;
    std::vector<int> itens_selecionados_exaustiva;
    auto start_exaustiva = std::chrono::high_resolution_clock::now();
    double max_value_exaustiva = knapsack(W, items, N, peso_total_exaustiva, itens_selecionados_exaustiva);
    auto end_exaustiva = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_exaustiva = end_exaustiva - start_exaustiva;

    std::cout << "Busca Exaustiva:" << std::endl;
    std::cout << "Valor máximo: " << max_value_exaustiva << std::endl;
    std::cout << "Peso total: " << peso_total_exaustiva << " de um peso máximo de: " << W << std::endl;
    std::cout << "Itens selecionados na mochila: ";
    for (int index : itens_selecionados_exaustiva) {
        std::cout << index << " ";
    }
    std::cout << std::endl;
    std::cout << "Tempo de execução: " << duration_exaustiva.count() << " segundos" << std::endl;

    // Heurística 1: Embaralhar e Preencher a Mochila
    std::cout << "\nHeurística 1: Embaralhar e Preencher a Mochila" << std::endl;
    for (int i = 0; i < 5; ++i) {
        double peso_total_heuristica1 = 0;
        std::vector<int> itens_selecionados_heuristica1;
        auto start_heuristica1 = std::chrono::high_resolution_clock::now();
        double max_value_heuristica1 = heuristic_shuffle_fill(W, items, peso_total_heuristica1, itens_selecionados_heuristica1);
        auto end_heuristica1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration_heuristica1 = end_heuristica1 - start_heuristica1;

        std::cout << "Execução " << i + 1 << ": Valor máximo: " << max_value_heuristica1
                  << ", Peso total: " << peso_total_heuristica1 << ", Tempo de execução: " << duration_heuristica1.count() << " segundos" << std::endl;
    }

    // Heurística 2: Seleção Aleatória Baseada em Probabilidade
    std::cout << "\nHeurística 2: Seleção Aleatória Baseada em Probabilidade" << std::endl;
    for (int i = 0; i < 5; ++i) {
        double peso_total_heuristica2 = 0;
        std::vector<int> itens_selecionados_heuristica2;
        auto start_heuristica2 = std::chrono::high_resolution_clock::now();
        double max_value_heuristica2 = heuristic_random_selection(W, items, peso_total_heuristica2, itens_selecionados_heuristica2, 0.5);
        auto end_heuristica2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration_heuristica2 = end_heuristica2 - start_heuristica2;

        std::cout << "Execução " << i + 1 << ": Valor máximo: " << max_value_heuristica2
                  << ", Peso total: " << peso_total_heuristica2 << ", Tempo de execução: " << duration_heuristica2.count() << " segundos" << std::endl;
    }

    return 0;
}
