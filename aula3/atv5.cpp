#include <iostream>

int main() {
    int nums[5];
    int sum = 0;

    for (int i = 0; i < 5; i++) {
        std::cout << "Digite o " << i + 1 << "º número: ";
        std::cin >> nums[i];
        sum += nums[i];
    }

    std::cout << "A soma dos números é " << sum << std::endl;

    return 0;
}

// Para compilar: g++ atv5.cpp -o atv5
// Para executar: srun --ntasks=1 --cpus-per-task=1 --mem=500M ./atv5