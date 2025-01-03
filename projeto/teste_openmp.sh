#!/bin/bash
#SBATCH --job-name=projeto        # Nome do job
#SBATCH --output=openmp.txt          # Nome do arquivo de saída
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --mem=4096
#SBATCH --time=02:00:00
#SBATCH --partition=espec

echo "Abordagem com OpenMP:"
./openmp
echo " "