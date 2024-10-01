#include <iostream>
#include <vector>
#include <algorithm> // para sort
#include <chrono> // para contagem de tempo

// Função que implementa a heurística para o problema do subconjunto de soma
bool subsetSum(const std::vector<int>& set, int sum) {
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

int main() {
    // Cenários de teste
    std::vector<int> set1 = {3, 34, 4, 12, 5, 2};
    int target1 = 9;

    std::vector<int> set2 = {1, 2, 3, 4, 5, 6};
    int target2 = 11;

    std::vector<int> set3 = {10, 20, 30};
    int target3 = 25;

    // Cenário 4: n = 1000
    std::vector<int> set4;
    int target4 = 5000;
    for (int i = 1; i <= 1000; ++i) {
        set4.push_back(i);
    }

    // Cenário 5: n = 10000
    std::vector<int> set5;
    int target5 = 200000;
    for (int i = 1; i <= 10000; ++i) {
        set5.push_back(i * 2);
    }

    // Cenário 6: n = 100000
    std::vector<int> set6;
    int target6 = 50000;
    for (int i = 1; i <= 100000; ++i) {
        set6.push_back(i);
    }

    // Testes com contagem de tempo
    auto start1 = std::chrono::high_resolution_clock::now();
    bool result1 = subsetSum(set1, target1);
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed1 = end1 - start1;
    std::cout << "Teste 1: " << (result1 ? "true" : "false") << " - Tempo: " << elapsed1.count() << " segundos" << std::endl;

    auto start2 = std::chrono::high_resolution_clock::now();
    bool result2 = subsetSum(set2, target2);
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed2 = end2 - start2;
    std::cout << "Teste 2: " << (result2 ? "true" : "false") << " - Tempo: " << elapsed2.count() << " segundos" << std::endl;

    auto start3 = std::chrono::high_resolution_clock::now();
    bool result3 = subsetSum(set3, target3);
    auto end3 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed3 = end3 - start3;
    std::cout << "Teste 3: " << (result3 ? "true" : "false") << " - Tempo: " << elapsed3.count() << " segundos" << std::endl;

    // Teste 4 com n = 5000
    auto start4 = std::chrono::high_resolution_clock::now();
    bool result4 = subsetSum(set4, target4);
    auto end4 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed4 = end4 - start4;
    std::cout << "Teste 6 (n = 5000): " << (result4 ? "true" : "false") << " - Tempo: " << elapsed4.count() << " segundos" << std::endl;

    // Teste 5 com n = 10000
    auto start5 = std::chrono::high_resolution_clock::now();
    bool result5 = subsetSum(set5, target5);
    auto end5 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed5 = end5 - start5;
    std::cout << "Teste 5 (n = 10000): " << (result5 ? "true" : "false") << " - Tempo: " << elapsed5.count() << " segundos" << std::endl;

    // Teste 6 com n = 100000
    auto start6 = std::chrono::high_resolution_clock::now();
    bool result6 = subsetSum(set6, target6);
    auto end6 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed6 = end6 - start6;
    std::cout << "Teste 6 (n = 100000): " << (result6 ? "true" : "false") << " - Tempo: " << elapsed6.count() << " segundos" << std::endl;

    return 0;
}
