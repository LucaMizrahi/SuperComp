#!/bin/bash
#SBATCH --job-name=job_simulado   # Nome do job
#SBATCH --output=q4_teste.txt     # Nome do arquivo de saída
#SBATCH --ntasks=4                # Número de processos
#SBATCH --cpus-per-task=5         # Número de threads por processo  
#SBATCH --mem=4096                # Quantidade de memória
#SBATCH --time=02:00:00           # Tempo máximo de execução
#SBATCH --partition=espec         # Fila (partition) a ser utilizada

# Exporta a variável de ambiente para definir o número de threads
export OMP_NUM_THREADS=10

mpirun -np 4 ./q4_teste