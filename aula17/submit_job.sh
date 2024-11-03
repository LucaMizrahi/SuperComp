#!/bin/bash
#SBATCH --job-name=exs_aula16          # Nome do job
#SBATCH --output=output_aula16.txt          # Nome do arquivo de saída
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --mem=4096
#SBATCH --time=03:00:00
#SBATCH --partition=espec

echo "Saída ex1:"
./ex1
echo " "

echo "Saída ex2:"
./ex2
echo " "

echo "Saída ex3:"
mpirun -np 4 ./ex3
echo " "

echo "Saída ex4:"
mpirun -np 4 ./ex4
echo " "