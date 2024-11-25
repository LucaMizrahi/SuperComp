#!/bin/bash
#SBATCH --job-name=job_simulado   # Nome do job
#SBATCH --output=q3_2.txt           # Nome do arquivo de saída
#SBATCH --ntasks=1                # Número de processos
#SBATCH --cpus-per-task=2         # Número de threads por processo  
#SBATCH --mem=4096                # Quantidade de memória (ou 3G)
#SBATCH --time=02:00:00           # Tempo máximo de execução
#SBATCH --partition=espec         # Fila (partition) a ser utilizada

# Exporta a variável de ambiente para definir o número de threads
export OMP_NUM_THREADS=2

./q3