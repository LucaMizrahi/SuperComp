#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>  // Biblioteca para medir o tempo de execução

// Estrutura para representar um item
struct item {
    int id;
    double peso;
    double valor;
};

// Função recursiva para resolver o problema da mochila
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

int main() {
    std::ifstream inputFile("entrada1.txt");
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

    double peso_total = 0;  // Peso total dos itens incluídos na mochila
    std::vector<int> itens_selecionados;  // Vetor para armazenar os índices dos itens selecionados

    // Medir o tempo de início
    auto start = std::chrono::high_resolution_clock::now();

    // Calcular o valor máximo que pode ser obtido
    double max_value = knapsack(W, items, N, peso_total, itens_selecionados);

    // Medir o tempo de fim
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "O valor máximo que pode ser obtido é: " << max_value << std::endl;
    std::cout << "O peso total ocupado é: " << peso_total << " de um peso máximo de: " << W << std::endl;

    std::cout << "Itens selecionados na mochila:" << std::endl;
    for (int index : itens_selecionados) {
        std::cout << "Item " << items[index].id << " (Peso: " << items[index].peso << ", Valor: " << items[index].valor << ")" << std::endl;
    }

    // Exibir o tempo de execução em segundos
    std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;

    return 0;
}
