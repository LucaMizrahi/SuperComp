#include <iostream>
#include <vector>
#include <chrono> // Biblioteca para medir o tempo

// Função auxiliar que usa memorização para calcular Fibonacci
int fibMemo(int n, std::vector<int>& memo) {
    if (n <= 1) {
        return n;
    }
    // Se o valor já foi calculado antes, retorná-lo
    if (memo[n] != -1) {
        return memo[n];
    }
    // Caso contrário, calculamos e armazenamos o resultado
    memo[n] = fibMemo(n - 1, memo) + fibMemo(n - 2, memo);
    return memo[n];
}

int fib(int n) {
    std::vector<int> memo(n + 1, -1); // Inicializamos o vetor de memorização com -1
    return fibMemo(n, memo);
}

int main() {
    int n;
    std::cout << "Digite o valor de N para calcular Fibonacci: ";
    std::cin >> n;

    // Medir o tempo de execução da função otimizada
    auto start = std::chrono::high_resolution_clock::now();
    int result = fib(n);
    auto end = std::chrono::high_resolution_clock::now();

    // Exibir o resultado e o tempo de execução
    std::cout << "Fibonacci de " << n << " é " << result << std::endl;
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Tempo de execução (com memorização): " << elapsed.count() << " segundos" << std::endl;

    return 0;
}
