#!/bin/bash
#SBATCH --job-name=teste_projeto        # Nome do job
#SBATCH --output=teste_openmp_gulosa.txt          # Nome do arquivo de saída
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --mem=4096
#SBATCH --time=02:00:00
#SBATCH --partition=espec

echo "Abordagem com OpenMP - heurística gulosa:"
./openmp_gulosa
echo " "