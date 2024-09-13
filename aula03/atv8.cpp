#include <iostream>

int main() {
    int mat[3][3];
    int sum = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << "Digite o elemento da linha " << i + 1 << " e coluna " << j + 1 << ": ";
            std::cin >> mat[i][j];
            if (i == j) {
                sum += mat[i][j];
            }
        }
    }

    std::cout << "A soma dos elementos na diagonal principal é " << sum << std::endl;

    return 0;
}

// Para compilar: g++ atv8.cpp -o atv8
// Para executar: srun --ntasks=1 --cpus-per-task=1 --mem=500M ./atv8