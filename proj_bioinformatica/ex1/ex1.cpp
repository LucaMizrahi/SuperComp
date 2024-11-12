#include <iostream>
#include <fstream>
#include <vector>
#include <mpi.h>
#include <omp.h>
#include <unordered_map>
#include <cctype> // Para a função tolower
#include <filesystem>

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

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Define o caminho para o arquivo na pasta databases
    std::string filename = "databases/chr1.subst.fa"; // Nome do arquivo de teste inicial
    std::ifstream file(filename);

    if (!file.is_open()) {
        if (world_rank == 0) {
            std::cerr << "Erro ao abrir o arquivo." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    // Cada processo lê uma porção do arquivo
    std::vector<std::string> sequences;
    std::string line, sequence_part;

    // Ignora linhas de cabeçalho (iniciadas com '>')
    if (world_rank == 0) {
        while (std::getline(file, line)) {
            if (line[0] != '>') {
                sequence_part += line;
            }
        }
    }

    // Dividir a sequência carregada entre os processos
    int chunk_size = sequence_part.size() / world_size;
    std::string local_sequence = sequence_part.substr(world_rank * chunk_size, chunk_size);

    if (world_rank == world_size - 1) {
        local_sequence += sequence_part.substr(world_rank * chunk_size + chunk_size);
    }

    file.close();

    // Cada processo conta as bases em sua parte da sequência
    std::unordered_map<char, int> local_count = countBases(local_sequence);

    // Agregação dos resultados com MPI_Reduce
    int global_count[4] = {0, 0, 0, 0}; // a, t, c, g
    int local_counts[4] = {local_count['a'], local_count['t'], local_count['c'], local_count['g']};

    MPI_Reduce(local_counts, global_count, 4, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        // Define o caminho para o arquivo de saída
        std::ofstream output_file("ex1/ex1.txt");
        
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
