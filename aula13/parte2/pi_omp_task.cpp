#include <omp.h>
#include <iostream>
#include <iomanip>

static long num_steps = 1024l * 1024 * 1024 * 2;  // Número de passos
#define MIN_BLK 1024 * 1024 * 64  // Modifique este valor para testar diferentes granularidades

double pi_r(long Nstart, long Nfinish, double step) {
    long i, iblk;
    double sum_local = 0.0;

    // Caso base: se o tamanho do bloco for menor que MIN_BLK
    if (Nfinish - Nstart < MIN_BLK) {
        for (i = Nstart; i < Nfinish; i++) {
            double x = (i + 0.5) * step;
            sum_local += 4.0 / (1.0 + x * x);
        }
    } else {
        iblk = Nfinish - Nstart;
        double sum1 = 0.0, sum2 = 0.0;

        #pragma omp task shared(sum1)
        sum1 = pi_r(Nstart, Nfinish - iblk / 2, step);

        #pragma omp task shared(sum2)
        sum2 = pi_r(Nfinish - iblk / 2, Nfinish, step);

        #pragma omp taskwait  // Sincronizar as tasks
        sum_local = sum1 + sum2;
    }

    return sum_local;
}

int main() {
    double step = 1.0 / (double)num_steps;
    double init_time, final_time;
    double sum = 0.0;

    // Teste com diferentes valores de MIN_BLK
    for (int i = 0; i < 3; i++) {
        sum = 0;  // Resetar a variável para cada execução
        std::cout << "Execução " << i + 1 << " com MIN_BLK = " << MIN_BLK << ":\n";

        init_time = omp_get_wtime();  // Início da medição de tempo

        #pragma omp parallel
        {
            #pragma omp single  // Apenas uma thread inicia as tasks
            {
                sum = pi_r(0, num_steps, step);
            }
        }

        double pi = step * sum;
        final_time = omp_get_wtime() - init_time;  // Fim da medição de tempo

        std::cout << "Para " << num_steps << " passos, pi = "
                  << std::setprecision(15) << pi
                  << " em " << final_time << " segundos\n";
    }

    return 0;
}
