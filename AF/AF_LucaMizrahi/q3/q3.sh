#!/bin/bash
#SBATCH --job-name=PF             # Nome do job
#SBATCH --output=q3.txt         # Nome do arquivo de saída
#SBATCH --ntasks=4                # Número de processos
#SBATCH --cpus-per-task=1         # Número de threads por processo  
#SBATCH --mem=4096                # Quantidade de memória (ou 3G)
#SBATCH --time=02:00:00           # Tempo máximo de execução
#SBATCH --partition=espec         # Fila (partition) a ser utilizada

# Exporta a variável de ambiente para definir o número de threads
export OMP_NUM_THREADS=4

./q3