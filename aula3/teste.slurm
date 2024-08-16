#!/bin/bash
#SBATCH --job-name=atv_aula3    # Nome do job
#SBATCH --output=atv_aula3_%j.txt   # Gera um novo arquivo de saída a cada execução
#SBATCH --ntasks=1                               # Número de tarefas (apenas 1 processos)
#SBATCH --cpus-per-task=1                        # Número de threads por tarefa (apenas 1 thread por processo)
#SBATCH --mem=1024MB                             # Memória total alocada por nó (1024 MB)
#SBATCH --time=00:10:00                          # Tempo máximo de execução (até 10 minutos)
#SBATCH --partition=espec                       # Fila do cluster a ser utilizada

# Executa o executável do atv1
echo "Executando atv1: "
./atv1
echo ""

# Executa o executável do atv2
# srun --ntasks=1 --cpus-per-task=1 --mem=500M ./atv2