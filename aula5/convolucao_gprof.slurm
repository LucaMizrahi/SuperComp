#!/bin/bash
#SBATCH --job-name=convolucao_teste    # Nome do job
#SBATCH --output=teste_%j.txt   # Gera um novo arquivo de saída a cada execução
#SBATCH --ntasks=4                               # Número de tarefas (apenas 1 processos)
#SBATCH --cpus-per-task=4                        # Número de threads por tarefa (apenas 1 thread por processo)
#SBATCH --mem=1024MB                             # Memória total alocada por nó (512 MB)
#SBATCH --time=00:10:00                          # Tempo máximo de execução (até 10 minutos)
#SBATCH --partition=espec                       # Fila do cluster a ser utilizada

# Executa o programa usando MPI e OpenMP
echo "Gproof"
./convolucao_gprof
