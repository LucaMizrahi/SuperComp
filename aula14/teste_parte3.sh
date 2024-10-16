#!/bin/bash
#SBATCH --job-name=parte3_test
#SBATCH --output=parte3.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --mem-per-cpu=1Gb
#SBATCH --time=00:05:00

./parte3
