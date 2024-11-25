#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <omp.h>

const int N1 = 500;   // Dimensão da primeira matriz
const int N2 = 1000;  // Dimensão da segunda matriz
const int N3 = 2000;  // Dimensão da terceira matriz

void matrixMultiply(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B, std::vector<std::vector<double>>& C) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < B[0].size(); j++) {
            C[i][j] = 0;
            for (int k = 0; k < A[0].size(); k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void testMatrixMultiplication(int matrixSize) {
    std::vector<std::vector<double>> A(matrixSize, std::vector<double>(matrixSize));
    std::vector<std::vector<double>> B(matrixSize, std::vector<double>(matrixSize));
    std::vector<std::vector<double>> C(matrixSize, std::vector<double>(matrixSize));

    srand(time(NULL));
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
        }
    }

    double start_time = omp_get_wtime();
    matrixMultiply(A, B, C);
    double time_taken = omp_get_wtime() - start_time;

    std::cout << "Matrix size: " << matrixSize << "x" << matrixSize << " - Time taken: " << time_taken << " seconds." << std::endl;
}

int main() {
    std::cout << "Running matrix multiplication tests...\n";

    std::cout << "Testing for matrix size " << N1 << ":\n";
    testMatrixMultiplication(N1);

    std::cout << "Testing for matrix size " << N2 << ":\n";
    testMatrixMultiplication(N2);

    std::cout << "Testing for matrix size " << N3 << ":\n";
    testMatrixMultiplication(N3);

    return 0;
}
