#include <iostream>
#include <chrono> // Biblioteca para medir o tempo

// Função recursiva simples para calcular Fibonacci
int fib(int n) {
    if (n <= 1) {
        return n;
    } else {
        return fib(n - 1) + fib(n - 2);
    }
}

int main() {
    int n;
    std::cout << "Digite o valor de N para calcular Fibonacci: ";
    std::cin >> n;

    // Medir o tempo de execução da função original (sem memorização)
    auto start = std::chrono::high_resolution_clock::now();
    int result = fib(n);
    auto end = std::chrono::high_resolution_clock::now();

    // Exibir o resultado e o tempo de execução
    std::cout << "Fibonacci de " << n << " é " << result << std::endl;
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Tempo de execução (sem memorização): " << elapsed.count() << " segundos" << std::endl;

    return 0;
}
