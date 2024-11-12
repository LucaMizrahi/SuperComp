#include <iostream>
#include <fstream>
#include <vector>
#include <mpi.h>
#include <omp.h>
#include <unordered_map>
#include <cctype> // Para a função tolower
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

// Função para contar bases em uma porção de sequência de DNA
std::unordered_map<char, int> countBases(const std::string& sequence) {
    std::unordered_map<char, int> baseCount = {{'a', 0}, {'t', 0}, {'c', 0}, {'g', 0}};
    
    #pragma omp parallel for
    for (size_t i = 0; i < sequence.size(); i++) {
        char base = std::tolower(sequence[i]); // Convertendo para minúscula
        if (baseCount.find(base) != baseCount.end()) {
            #pragma omp atomic
            baseCount[base]++;
        }
    }
    
    return baseCount;
}

// Função para ler um arquivo FASTA e retornar a sequência de DNA
std::string readFastaFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line, sequence;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line[0] != '>') { // Ignora linhas de cabeçalho
                sequence += line;
            }
        }
        file.close();
    } else {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
    }
    return sequence;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Vetor para acumular as contagens locais de cada base
    int local_counts[4] = {0, 0, 0, 0}; // a, t, c, g

    // Processamento dos arquivos de chr1 a chr22
    for (int i = 1; i <= 22; i++) {
        // Define o caminho do arquivo
        std::string filename = "databases/chr" + std::to_string(i) + ".subst.fa";
        
        // O processo mestre lê o arquivo e distribui a sequência entre os processos
        std::string sequence_part;
        if (world_rank == 0) {
            sequence_part = readFastaFile(filename);
        }

        // Divide a sequência entre os processos
        int chunk_size = sequence_part.size() / world_size;
        std::string local_sequence = sequence_part.substr(world_rank * chunk_size, chunk_size);
        
        // O último processo pega qualquer caractere restante
        if (world_rank == world_size - 1) {
            local_sequence += sequence_part.substr(world_rank * chunk_size + chunk_size);
        }

        // Contagem das bases na sequência local
        std::unordered_map<char, int> local_count = countBases(local_sequence);
        
        // Armazena a contagem local em um array para MPI_Reduce
        local_counts[0] += local_count['a'];
        local_counts[1] += local_count['t'];
        local_counts[2] += local_count['c'];
        local_counts[3] += local_count['g'];
    }

    // Agregação dos resultados de todos os processos
    int global_count[4] = {0, 0, 0, 0}; // a, t, c, g
    MPI_Reduce(local_counts, global_count, 4, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Escreve a contagem total no arquivo ex1/ex1.txt
    if (world_rank == 0) {
        std::ofstream output_file("ex1/ex1_full.txt");
        
        if (output_file.is_open()) {
            output_file << "Contagem de bases totais:" << std::endl;
            output_file << "A: " << global_count[0] << std::endl;
            output_file << "T: " << global_count[1] << std::endl;
            output_file << "C: " << global_count[2] << std::endl;
            output_file << "G: " << global_count[3] << std::endl;
            output_file.close();
        } else {
            std::cerr << "Erro ao abrir o arquivo de saída." << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}
