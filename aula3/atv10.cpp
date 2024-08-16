#include <iostream>

int main() {
    std::string items[100];
    int quantities[100];
    int n_items = 0;

    while (true) {
        std::cout << "1. Adicionar item" << std::endl;
        std::cout << "2. Remover item" << std::endl;
        std::cout << "3. Visualizar estoque" << std::endl;
        std::cout << "4. Sair" << std::endl;
        std::cout << "Escolha uma opção: ";
        int option;
        std::cin >> option;

        if (option == 1) {
            std::cout << "Digite o nome do item: ";
            std::string item;
            std::cin >> item;
            items[n_items] = item;
            std::cout << "Digite a quantidade do item: ";
            int quantity;
            std::cin >> quantity;
            quantities[n_items] = quantity;
            n_items++;
        } else if (option == 2) {
            std::cout << "Digite o nome do item que deseja remover: ";
            std::string item;
            std::cin >> item;
            for (int i = 0; i < n_items; i++) {
                if (items[i] == item) {
                    for (int j = i; j < n_items - 1; j++) {
                        items[j] = items[j + 1];
                        quantities[j] = quantities[j + 1];
                    }
                    n_items--;
                    break;
                }
            }
        } else if (option == 3) {
            for (int i = 0; i < n_items; i++) {
                std::cout << items[i] << ": " << quantities[i] << std::endl;
            }
        } else if (option == 4) {
            break;
        } else {
            std::cout << "Opção inválida" << std::endl;
        }
    }

    return 0;
}

// Para compilar: g++ atv10.cpp -o atv10
// Para executar: srun --ntasks=1 --cpus-per-task=1 --mem=500M ./atv10