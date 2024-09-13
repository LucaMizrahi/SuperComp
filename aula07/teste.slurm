#!/bin/bash
#SBATCH --job-name=mochila_teste    # Nome do job
#SBATCH --output=teste_%j.txt   # Gera um novo arquivo de saída a cada execução
#SBATCH --ntasks=1                               # Número de tarefas (apenas 1 processos)
#SBATCH --cpus-per-task=1                        # Número de threads por tarefa (apenas 1 thread por processo)
#SBATCH --mem=4096MB                             # Memória total alocada por nó (512 MB)
#SBATCH --time=02:00:00                          # Tempo máximo de execução (até 10 minutos)
#SBATCH --partition=espec                       # Fila do cluster a ser utilizada

# Executa o programa usando MPI e OpenMP
echo "Executando problema da mochila"
./main
echo " "