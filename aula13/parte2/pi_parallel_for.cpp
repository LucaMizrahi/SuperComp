#include <omp.h>
#include <iostream>
#include <iomanip>

static long num_steps = 1024l * 1024 * 1024 * 2;  // Número de passos
#define MIN_BLK 1024 * 1024 * 256  // Modifique este valor para testar diferentes granularidades

double sum = 0;

void pi_r(long Nstart, long Nfinish, double step) {
    long i;

    // Caso base: se o tamanho do bloco for menor que MIN_BLK, paralelize com for
    if (Nfinish - Nstart < MIN_BLK) {
        #pragma omp parallel for reduction(+:sum)
        for (i = Nstart; i < Nfinish; i++) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
    } else {
        long iblk = Nfinish - Nstart;
        pi_r(Nstart, Nfinish - iblk / 2, step);
        pi_r(Nfinish - iblk / 2, Nfinish, step);
    }
}

int main() {
    double step = 1.0 / (double)num_steps;
    double init_time, final_time;

    // Teste com diferentes valores de MIN_BLK
    for (int i = 0; i < 3; i++) {
        sum = 0;  // Resetar a variável global para cada execução
        std::cout << "Execução " << i + 1 << " com MIN_BLK = " << MIN_BLK << ":\n";

        init_time = omp_get_wtime();  // Início da medição de tempo

        pi_r(0, num_steps, step);

        double pi = step * sum;
        final_time = omp_get_wtime() - init_time;  // Fim da medição de tempo

        std::cout << "Para " << num_steps << " passos, pi = "
                  << std::setprecision(15) << pi
                  << " em " << final_time << " segundos\n";
    }

    return 0;
}
