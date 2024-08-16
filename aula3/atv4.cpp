// Escreva um programa que peça ao usuário para inserir uma string e exiba o número de
// caracteres na string

#include <iostream>

int main() {
    std::string str;

    std::cout << "Digite uma string: ";
    std::cin >> str;

    std::cout << "A string tem " << str.size() << " caracteres." << std::endl;

    return 0;
}

// Para compilar: g++ atv4.cpp -o atv4
// Para executar: srun --ntasks=1 --cpus-per-task=1 --mem=500M ./atv4