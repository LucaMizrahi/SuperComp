#include <iostream>
#include <vector>
#include <algorithm>  // Para std::sort
#include <fstream>    // Para manipulação de arquivos
#include <chrono>     // Para calcular o tempo de execução

// Estrutura para representar um item
struct item {
    int id;
    double peso;
    double valor;

    // Função que retorna a razão valor/peso (não será usada aqui, mas pode ser útil)
    double valor_por_peso() const {
        return valor / peso;
    }
};

// Função que implementa a heurística "Mais Caro Primeiro"
double mais_caro_primeiro(double W, std::vector<item>& items, double& peso_total, std::vector<int>& itens_selecionados) {
    // Ordena os itens pelo valor em ordem decrescente
    std::sort(items.begin(), items.end(), [](const item& a, const item& b) {
        return a.valor > b.valor;  // Ordena por valor decrescente
    });

    double valor_total = 0;
    peso_total = 0;
    itens_selecionados.clear();

    // Itera sobre os itens e adiciona à mochila se couber
    for (const auto& it : items) {
        if (peso_total + it.peso <= W) {
            peso_total += it.peso;
            valor_total += it.valor;
            itens_selecionados.push_back(it.id);  // Adiciona o ID do item à mochila
        }
    }

    return valor_total;  // Retorna o valor total dos itens na mochila
}

// Função que implementa a heurística "Mais Leve Primeiro"
double mais_leve_primeiro(double W, std::vector<item>& items, double& peso_total, std::vector<int>& itens_selecionados) {
    // Ordena os itens pelo peso em ordem crescente
    std::sort(items.begin(), items.end(), [](const item& a, const item& b) {
        return a.peso < b.peso;  // Ordena por peso crescente
    });

    double valor_total = 0;
    peso_total = 0;
    itens_selecionados.clear();

    // Itera sobre os itens e adiciona à mochila se couber
    for (const auto& it : items) {
        if (peso_total + it.peso <= W) {
            peso_total += it.peso;
            valor_total += it.valor;
            itens_selecionados.push_back(it.id);  // Adiciona o ID do item à mochila
        }
    }

    return valor_total;  // Retorna o valor total dos itens na mochila
}

int main() {
    std::ifstream inputFile("entrada1.txt");
    std::ofstream outputFile("saida1.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de entrada!" << std::endl;
        return 1;
    }

    if (!outputFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de saida!" << std::endl;
        return 1;
    }

    int N;  // Número de itens
    double W;  // Capacidade da mochila

    // Leitura da entrada
    inputFile >> N >> W;

    std::vector<item> items(N);
    for (int i = 0; i < N; ++i) {
        items[i].id = i + 1;
        inputFile >> items[i].peso >> items[i].valor;
    }

    inputFile.close();

    // Variáveis para armazenar resultados
    double peso_total_caro = 0, peso_total_leve = 0;
    std::vector<int> itens_selecionados_caro, itens_selecionados_leve;
    double valor_total_caro = 0, valor_total_leve = 0;

    // Medir o tempo de execução para "Mais Caro Primeiro"
    auto start_caro = std::chrono::high_resolution_clock::now();
    valor_total_caro = mais_caro_primeiro(W, items, peso_total_caro, itens_selecionados_caro);
    auto end_caro = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_caro = end_caro - start_caro;

    // Medir o tempo de execução para "Mais Leve Primeiro"
    auto start_leve = std::chrono::high_resolution_clock::now();
    valor_total_leve = mais_leve_primeiro(W, items, peso_total_leve, itens_selecionados_leve);
    auto end_leve = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_leve = end_leve - start_leve;

    // Escrever os resultados no arquivo de saída para "Mais Caro Primeiro"
    outputFile << "Heurística: Mais Caro Primeiro\n";
    outputFile << "Valor total dos itens na mochila: " << valor_total_caro << "\n";
    outputFile << "Peso total dos itens na mochila: " << peso_total_caro << "\n";
    outputFile << "Tempo de execução: " << duration_caro.count() << " segundos\n";
    outputFile << "Itens selecionados: ";
    for (int id : itens_selecionados_caro) {
        outputFile << id << " ";
    }
    outputFile << "\n\n";

    // Escrever os resultados no arquivo de saída para "Mais Leve Primeiro"
    outputFile << "Heurística: Mais Leve Primeiro\n";
    outputFile << "Valor total dos itens na mochila: " << valor_total_leve << "\n";
    outputFile << "Peso total dos itens na mochila: " << peso_total_leve << "\n";
    outputFile << "Tempo de execução: " << duration_leve.count() << " segundos\n";
    outputFile << "Itens selecionados: ";
    for (int id : itens_selecionados_leve) {
        outputFile << id << " ";
    }
    outputFile << "\n";

    outputFile.close();

    return 0;
}
