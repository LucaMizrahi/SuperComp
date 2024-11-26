#include <iostream>
#include <vector>
#include <omp.h>

// Função para processar a imagem (paralelizada)
void processImage(std::vector<std::vector<int>>& image) {
    size_t rows = image.size();

    // Paraleliza as linhas
    #pragma omp parallel for
    for (size_t i = 0; i < rows; ++i) {
        size_t cols = image[i].size();
        for (size_t j = 0; j < cols; ++j) {
            image[i][j] = (image[i][j] * 2) % 256;
        }
    }
}

int main() {
    // Imagem inicial representada como uma matriz 3x3
    std::vector<std::vector<int>> img{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    // Exibe a imagem original
    std::cout << "Imagem original:" << std::endl;
    for (const auto& row : img) {
        for (const auto& col : row) {
            std::cout << col << " ";
        }
        std::cout << std::endl;
    }

    // Processa a imagem (paralelizado)
    processImage(img);

    // Exibe a imagem processada
    std::cout << "\nImagem processada:" << std::endl;
    for (const auto& row : img) {
        for (const auto& col : row) {
            std::cout << col << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
