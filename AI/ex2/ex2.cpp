#include <iostream>
#include <vector>
#include <chrono> // para medir o tempo
#include <cstdlib> // para gerar números aleatórios
#include <ctime>   // para inicializar a seed do rand

bool subsetSumRecursive(const std::vector<int>& set, int n, int sum) {
    // Caso base: se a soma for 0, o subconjunto é encontrado
    if (sum == 0) return true;
    
    // Caso base: se não há mais elementos e a soma não foi encontrada
    if (n == 0 && sum != 0) return false;

    // Se o último elemento for maior que a soma, ignorá-lo
    if (set[n-1] > sum) return subsetSumRecursive(set, n-1, sum);

    // Verificar se a soma pode ser encontrada
    return subsetSumRecursive(set, n-1, sum) || subsetSumRecursive(set, n-1, sum - set[n-1]);
}

int main() {
    std::srand(std::time(0));

    // Definindo 10 tamanhos para os conjuntos de teste, com tamanhos maiores para entradas médias e grandes
    std::vector<int> setSizes = {6, 6, 50, 100, 500, 1000, 5000, 10000, 20000, 30000};

    // Definindo os alvos para cada conjunto aleatório
    std::vector<int> targets = {9, 11, 500, 1000, 300, 500, 1000, 1000, 1000, 1000};

    // Gerar 10 conjuntos aleatórios e medir o tempo de execução da solução ótima
    for (int i = 0; i < setSizes.size(); ++i) {
        // Gerar um conjunto aleatório de tamanho setSizes[i]
        std::vector<int> testSet(setSizes[i]);
        for (int j = 0; j < setSizes[i]; ++j) {
            testSet[j] = std::rand() % 100 + 1; // Números aleatórios entre 1 e 100
        }

        // Exibir o conjunto gerado para referência (limitar a exibição para conjuntos menores)
        if (setSizes[i] <= 50) {
            std::cout << "Conjunto " << (i + 1) << ": ";
            for (int num : testSet) {
                std::cout << num << " ";
            }
            std::cout << " - Alvo: " << targets[i] << std::endl;
        } else {
            std::cout << "Conjunto " << (i + 1) << " (Tamanho: " << setSizes[i] << ") - Alvo: " << targets[i] << std::endl;
        }

        // Medir o tempo de execução da solução ótima
        auto start = std::chrono::high_resolution_clock::now();
        bool result = subsetSumRecursive(testSet, testSet.size(), targets[i]);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        // Exibir o resultado e o tempo de execução
        std::cout << "Resultado: " << (result ? "true" : "false") 
                  << " - Tempo: " << elapsed.count() << " segundos" << std::endl;
        std::cout << "---------------------------------------------------------" << std::endl;
    }

    return 0;
}
