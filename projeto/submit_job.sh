#!/bin/bash
#SBATCH --job-name=projeto        # Nome do job
#SBATCH --output=projeto.txt          # Nome do arquivo de saída
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=4096
#SBATCH --time=02:00:00

echo "Tarefa 1:"
./exaustiva