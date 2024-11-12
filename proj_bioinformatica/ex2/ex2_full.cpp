#include <iostream>
#include <fstream>
#include <vector>
#include <mpi.h>
#include <omp.h>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

// Função para transcrever uma sequência de DNA para RNA
std::string transcribeDNAtoRNA(const std::string& dna_sequence) {
    std::string rna_sequence(dna_sequence.size(), ' ');
    
    #pragma omp parallel for
    for (size_t i = 0; i < dna_sequence.size(); i++) {
        char base = dna_sequence[i];
        switch (base) {
            case 'A':
                rna_sequence[i] = 'U';
                break;
            case 'T':
                rna_sequence[i] = 'A';
                break;
            case 'C':
                rna_sequence[i] = 'G';
                break;
            case 'G':
                rna_sequence[i] = 'C';
                break;
            default:
                rna_sequence[i] = ' '; // Ignora caracteres inválidos
                break;
        }
    }
    
    return rna_sequence;
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

    // Cria a pasta de saída "outputs" dentro de "ex2" se ela não existir
    if (world_rank == 0) {
        fs::create_directories("ex2/outputs");
    }
    MPI_Barrier(MPI_COMM_WORLD); // Sincroniza todos os processos após a criação da pasta

    // Processamento dos arquivos de chr1 a chr22
    for (int i = 1; i <= 22; i++) {
        // Define o caminho do arquivo de entrada e o caminho do arquivo de saída
        std::string filename = "databases/chr" + std::to_string(i) + ".subst.fa";
        std::string output_filename = "ex2/outputs/chr" + std::to_string(i) + "_rna.txt";
        
        // O processo mestre lê o arquivo e distribui a sequência entre os processos
        std::string dna_sequence;
        if (world_rank == 0) {
            dna_sequence = readFastaFile(filename);
        }

        // Divide a sequência entre os processos
        int chunk_size = dna_sequence.size() / world_size;
        std::string local_dna_sequence = dna_sequence.substr(world_rank * chunk_size, chunk_size);
        
        // O último processo pega qualquer caractere restante
        if (world_rank == world_size - 1) {
            local_dna_sequence += dna_sequence.substr(world_rank * chunk_size + chunk_size);
        }

        // Transcrição da sequência local de DNA para RNA
        std::string local_rna_sequence = transcribeDNAtoRNA(local_dna_sequence);

        // Agrega as sequências transcritas em uma única sequência no processo mestre
        std::vector<int> recv_counts(world_size);
        int local_size = local_rna_sequence.size();
        MPI_Gather(&local_size, 1, MPI_INT, recv_counts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

        std::vector<int> displs(world_size, 0);
        std::string rna_sequence_from_file;
        if (world_rank == 0) {
            int total_size = 0;
            for (int j = 0; j < world_size; ++j) {
                displs[j] = total_size;
                total_size += recv_counts[j];
            }
            rna_sequence_from_file.resize(total_size);
        }

        MPI_Gatherv(local_rna_sequence.data(), local_size, MPI_CHAR, &rna_sequence_from_file[0], recv_counts.data(), displs.data(), MPI_CHAR, 0, MPI_COMM_WORLD);

        // Escreve a sequência RNA transcrita em um arquivo específico para cada cromossomo
        if (world_rank == 0) {
            std::ofstream output_file(output_filename);
            
            if (output_file.is_open()) {
                output_file << "Sequência RNA transcrita para chr" << i << ":" << std::endl;
                output_file << rna_sequence_from_file << std::endl;
                output_file.close();
            } else {
                std::cerr << "Erro ao abrir o arquivo de saída: " << output_filename << std::endl;
            }
        }
    }

    MPI_Finalize();
    return 0;
}
