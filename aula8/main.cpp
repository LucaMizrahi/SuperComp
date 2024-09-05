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

// Algoritmo de Busca Local: Mochila Cheia
double mochila_cheia(double W, const std::vector<item>& items, double& peso_total, std::vector<int>& itens_selecionados) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, items.size() - 1);

    double valor_total = 0;
    peso_total = 0;
    itens_selecionados.clear();
    std::vector<bool> usados(items.size(), false);
    
    // Passo 1: gerar solução aleatória
    for (size_t i = 0; i < items.size(); ++i) {
        if (peso_total + items[i].peso <= W) {
            peso_total += items[i].peso;
            valor_total += items[i].valor;
            itens_selecionados.push_back(items[i].id);
            usados[i] = true;
        }
    }

    // Passo 2: percorrer novamente todos os objetos
    for (size_t i = 0; i < items.size(); ++i) {
        if (!usados[i] && peso_total + items[i].peso <= W) {
            peso_total += items[i].peso;
            valor_total += items[i].valor;
            itens_selecionados.push_back(items[i].id);
            usados[i] = true;
        }
    }

    return valor_total;
}

// Algoritmo de Busca Local: Substituição de Objeto
double substituicao_objeto(double W, const std::vector<item>& items, double& peso_total, std::vector<int>& itens_selecionados) {
    // Inicializa a solução aleatória
    double valor_total = mochila_cheia(W, items, peso_total, itens_selecionados);

    bool melhorou;
    do {
        melhorou = false;
        for (size_t i = 0; i < items.size(); ++i) {
            if (std::find(itens_selecionados.begin(), itens_selecionados.end(), items[i].id) != itens_selecionados.end()) continue;  // Pula os itens já usados

            for (size_t j = 0; j < itens_selecionados.size(); ++j) {
                double peso_novo = peso_total - items[itens_selecionados[j] - 1].peso + items[i].peso;
                double valor_novo = valor_total - items[itens_selecionados[j] - 1].valor + items[i].valor;

                if (peso_novo <= W && valor_novo > valor_total) {
                    peso_total = peso_novo;
                    valor_total = valor_novo;
                    itens_selecionados[j] = items[i].id;  // Troca o item
                    melhorou = true;
                    break;
                }
            }
            if (melhorou) break;  // Reinicia a busca se houve melhoria
        }
    } while (melhorou);

    return valor_total;
}

// Algoritmo de Busca Local: Hill Climbing
double hill_climbing(double W, const std::vector<item>& items, double& peso_total, std::vector<int>& itens_selecionados) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    std::vector<int> solucao(items.size(), 0);
    peso_total = 0;
    double valor_total = 0;

    // Gera uma solução inicial aleatória
    for (size_t i = 0; i < items.size(); ++i) {
        solucao[i] = dis(gen);
        if (solucao[i] == 1 && peso_total + items[i].peso <= W) {
            peso_total += items[i].peso;
            valor_total += items[i].valor;
            itens_selecionados.push_back(items[i].id);
        } else {
            solucao[i] = 0;  // Não pode adicionar o item devido ao limite de peso
        }
    }

    bool melhorou;
    do {
        melhorou = false;
        for (size_t i = 0; i < items.size(); ++i) {
            if (solucao[i] == 0 && peso_total + items[i].peso <= W) {
                solucao[i] = 1;  // Tenta adicionar o item
                double novo_valor_total = valor_total + items[i].valor;
                if (novo_valor_total > valor_total) {
                    valor_total = novo_valor_total;
                    peso_total += items[i].peso;
                    itens_selecionados.push_back(items[i].id);
                    melhorou = true;
                } else {
                    solucao[i] = 0;  // Não houve melhoria, reverte a adição
                }
            }
        }
    } while (melhorou);

    return valor_total;
}

int main() {
    std::ifstream inputFile("entrada4.txt");
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

    // Algoritmo: Mochila Cheia
    std::cout << "\nAlgoritmo: Mochila Cheia" << std::endl;
    for (int i = 0; i < 5; ++i) {
        double peso_total_cheia = 0;
        std::vector<int> itens_selecionados_cheia;
        auto start_cheia = std::chrono::high_resolution_clock::now();
        double max_value_cheia = mochila_cheia(W, items, peso_total_cheia, itens_selecionados_cheia);
        auto end_cheia = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration_cheia = end_cheia - start_cheia;

        std::cout << "Execução " << i + 1 << ": Valor máximo: " << max_value_cheia
                  << ", Peso total: " << peso_total_cheia << ", Tempo de execução: " << duration_cheia.count() << " segundos" << std::endl;
    }

    // Algoritmo: Substituição de Objeto
    std::cout << "\nAlgoritmo: Substituição de Objeto" << std::endl;
    for (int i = 0; i < 5; ++i) {
        double peso_total_substituicao = 0;
        std::vector<int> itens_selecionados_substituicao;
        auto start_substituicao = std::chrono::high_resolution_clock::now();
        double max_value_substituicao = substituicao_objeto(W, items, peso_total_substituicao, itens_selecionados_substituicao);
        auto end_substituicao = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration_substituicao = end_substituicao - start_substituicao;

        std::cout << "Execução " << i + 1 << ": Valor máximo: " << max_value_substituicao
                  << ", Peso total: " << peso_total_substituicao << ", Tempo de execução: " << duration_substituicao.count() << " segundos" << std::endl;
    }

    // Algoritmo: Hill Climbing
    std::cout << "\nAlgoritmo: Hill Climbing" << std::endl;
    for (int i = 0; i < 5; ++i) {
        double peso_total_hill = 0;
        std::vector<int> itens_selecionados_hill;
        auto start_hill = std::chrono::high_resolution_clock::now();
        double max_value_hill = hill_climbing(W, items, peso_total_hill, itens_selecionados_hill);
        auto end_hill = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration_hill = end_hill - start_hill;

        std::cout << "Execução " << i + 1 << ": Valor máximo: " << max_value_hill
                  << ", Peso total: " << peso_total_hill << ", Tempo de execução: " << duration_hill.count() << " segundos" << std::endl;
    }

    return 0;
}
