#!/bin/bash
#SBATCH --job-name=job_prova      # Nome do job
#SBATCH --output=q1_prova.txt      # Nome do arquivo de saída
#SBATCH --nodes=5                 # Número de máquinas
#SBATCH --tasks-per-node=1        # Número de tarefas por máquina
#SBATCH --cpus-per-task=4         # Número de threads por tarefa
#SBATCH --mem=16G                 # Quantidade de memória (ou 16G)
#SBATCH --time=01:00:00           # Tempo máximo de execução
#SBATCH --partition=prova         # Fila (partition) a ser utilizada

# Exporta a variável de ambiente para definir o número de threads
export OMP_NUM_THREADS=15

./programa