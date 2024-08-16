// Crie um programa que peça ao usuário para inserir dois números e uma operação (adição, subtração,
//multiplicação ou divisão). O programa deve exibir o resultado da operação.

#include <iostream>

int main() {
    float num1, num2;
    char op;

    std::cout << "Digite o primeiro número: ";
    std::cin >> num1;

    std::cout << "Digite o segundo número: ";
    std::cin >> num2;

    std::cout << "Digite a operação (+, -, *, /): ";
    std::cin >> op;

    switch (op) {
        case '+':
            std::cout << num1 << " + " << num2 << " = " << num1 + num2 << std::endl;
            break;
        case '-':
            std::cout << num1 << " - " << num2 << " = " << num1 - num2 << std::endl;
            break;
        case '*':
            std::cout << num1 << " * " << num2 << " = " << num1 * num2 << std::endl;
            break;
        case '/':
            std::cout << num1 << " / " << num2 << " = " << num1 / num2 << std::endl;
            break;
        default:
            std::cout << "Operação inválida." << std::endl;
    }

    return 0;
}