// Escreva um programa que peça ao usuário para inserir 10 números em um vetor e determine qual é o maior número.

#include <iostream>

int main() {
    int nums[10];
    int max = 0;

    for (int i = 0; i < 10; i++) {
        std::cout << "Digite o " << i + 1 << "º número: ";
        std::cin >> nums[i];

        if (nums[i] > max) {
            max = nums[i];
        }
    }

    std::cout << "O maior número é " << max << std::endl;

    return 0;
}

// Para compilar: g++ atv6.cpp -o atv6
// Para executar: srun --ntasks=1 --cpus-per-task=1 --mem=500M ./atv6