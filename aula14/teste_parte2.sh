#!/bin/bash
#SBATCH --job-name=parte2_test
#SBATCH --output=parte2.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --mem-per-cpu=1Gb
#SBATCH --time=00:05:00

./parte2