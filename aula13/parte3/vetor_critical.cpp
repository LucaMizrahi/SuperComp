#include <vector>
#include <iostream>
#include <omp.h>

// Função para realizar uma operação de cálculo "complexa"
double conta_complexa(int i) {
    return 2 * i;
}

int main() {
    int N = 1000000;  // Tamanho do vetor
    std::vector<double> vec;

    for (int exec = 0; exec < 3; exec++) {  // Executar 3 vezes
        vec.clear();  // Limpa o vetor para cada execução

        double start_time = omp_get_wtime();  // Início da medição do tempo

        #pragma omp parallel for
        for (int i = 0; i < N; i++) {
            #pragma omp critical  // Impede acessos simultâneos ao vetor
            {
                vec.push_back(conta_complexa(i));
            }
        }

        double end_time = omp_get_wtime();  // Fim da medição do tempo
        double elapsed_time = end_time - start_time;

        std::cout << "Tempo de execução (com omp critical), execução " << exec + 1 << ": " 
                  << elapsed_time << " segundos\n";
    }

    return 0;
}
