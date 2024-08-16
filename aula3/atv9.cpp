#include <iostream>

int main() {
    std::string str;
    std::cout << "Digite uma string: ";
    std::cin >> str;

    bool is_palindrome = true;
    for (int i = 0; i < str.size() / 2; i++) {
        if (str[i] != str[str.size() - i - 1]) {
            is_palindrome = false;
            break;
        }
    }

    if (is_palindrome) {
        std::cout << "A string é um palíndromo" << std::endl;
    } else {
        std::cout << "A string não é um palíndromo" << std::endl;
    }

    return 0;
}

// Para compilar: g++ atv9.cpp -o atv9
// Para executar: srun --ntasks=1 --cpus-per-task=1 --mem=500M ./atv9