#!/bin/bash
#SBATCH --job-name=projeto_bio        # Nome do job
#SBATCH --output=ex3.txt          # Nome do arquivo de saída
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --mem=4096
#SBATCH --time=02:00:00
#SBATCH --partition=espec

echo "Ex1:"
mpirun -np 4 ./ex3
echo " "