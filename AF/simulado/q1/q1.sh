#!/bin/bash
#SBATCH --job-name=job_prova      # Nome do job
#SBATCH --output=projeto.txt      # Nome do arquivo de saída
#SBATCH --nodes=2                 # Número de máquinas
#SBATCH --tasks-per-node=5        # Número de tarefas por máquina
#SBATCH --mem=3072                # Quantidade de memória (ou 3G)
#SBATCH --time=02:00:00           # Tempo máximo de execução
#SBATCH --partition=prova         # Fila (partition) a ser utilizada

# Exporta a variável de ambiente para definir o número de threads
export OMP_NUM_THREADS=10

./executavel