#include <iostream>

int main() {
    int nums[5] = {5, 3, 2, 4, 1};
    int n = 5;

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (nums[j] > nums[j + 1]) {
                int temp = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        std::cout << nums[i] << " ";
    }

    std::cout << std::endl;

    return 0;
}

// Para compilar: g++ atv7.cpp -o atv7
// Para executar: srun --ntasks=1 --cpus-per-task=1 --mem=500M ./atv7