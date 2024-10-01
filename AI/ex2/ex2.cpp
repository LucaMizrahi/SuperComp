#include <iostream>
#include <vector>
#include <algorithm> // para sort
#include <chrono> // para contagem de tempo
#include <cstdlib> // para rand() e srand()
#include <ctime>   // para time()

// Função que implementa a heurística para o problema do subconjunto de soma
bool subsetSumHeuristic(const std::vector<int>& set, int sum) {
    std::vector<int> sortedSet = set;

    // Passo 1: Ordenar o conjunto em ordem decrescente
    std::sort(sortedSet.begin(), sortedSet.end(), std::greater<int>());

    int currentSum = 0;
    
    // Passo 2: Iterar sobre os elementos e decidir se inclui no subconjunto
    for (int num : sortedSet) {
        if (currentSum + num <= sum) {
            currentSum += num;
        }

        // Passo 3: Verificar se a soma já atingiu o valor alvo
        if (currentSum == sum) {
            return true;
        }
    }

    // Se não for possível alcançar a soma alvo, retorne falso
    return false;
}

// Função para gerar conjuntos aleatórios
std::vector<int> generateRandomSet(int n, int maxVal) {
    std::vector<int> set(n);
    for (int i = 0; i < n; ++i) {
        set[i] = rand() % maxVal + 1;  // Números aleatórios entre 1 e maxVal
    }
    return set;
}

int main() {
    // Inicializar a semente do gerador de números aleatórios
    srand(static_cast<unsigned int>(time(0)));

    // Definir tamanhos dos conjuntos e os valores alvo
    std::vector<int> setSizes = {10, 20, 30, 40, 50, 100, 200, 500, 1000, 10000};
    std::vector<int> targets = {50, 100, 150, 200, 300, 500, 800, 1000, 1500, 10000};

    // Executando os testes com conjuntos aleatórios
    for (int i = 0; i < setSizes.size(); ++i) {
        // Gerar o conjunto aleatório com o tamanho especificado
        std::vector<int> randomSet = generateRandomSet(setSizes[i], 100);  // maxVal = 100 para variabilidade

        // Obter o valor alvo correspondente
        int target = targets[i];

        // Medir o tempo de execução da heurística
        auto start = std::chrono::high_resolution_clock::now();
        bool result = subsetSumHeuristic(randomSet, target);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        // Imprimir o resultado
        std::cout << "Teste " << (i + 1) << " (Tamanho do conjunto: " << setSizes[i] 
                  << ", Alvo: " << target << "): "
                  << (result ? "true" : "false") << " - Tempo: " << elapsed.count() << " segundos" << std::endl;
    }

    return 0;
}
