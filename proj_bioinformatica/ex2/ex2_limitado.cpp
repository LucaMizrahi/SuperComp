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

// Função para ler uma sequência válida de DNA do arquivo FASTA
std::string readValidFastaSequence(const std::string& filename, size_t max_chars) {
    std::ifstream file(filename);
    std::string line, sequence;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line[0] != '>') { // Ignora linhas de cabeçalho
                for (char base : line) {
                    // Adiciona apenas caracteres válidos de DNA
                    if (base == 'A' || base == 'T' || base == 'C' || base == 'G') {
                        sequence += base;
                    }
                    // Interrompe a coleta se atingir o número máximo de caracteres válidos
                    if (sequence.size() >= max_chars) {
                        break;
                    }
                }
                if (sequence.size() >= max_chars) {
                    break;
                }
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

    // Define o caminho para o arquivo específico de teste (chr1.subst.fa)
    std::string filename = "databases/chr1.subst.fa";

    // Define o número máximo de caracteres para o teste
    const size_t max_test_chars = 200; // Altere este valor para o número desejado de caracteres

    // O processo mestre lê uma sequência válida do arquivo para teste e distribui entre os processos
    std::string dna_sequence;
    if (world_rank == 0) {
        dna_sequence = readValidFastaSequence(filename, max_test_chars);
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

    // Agrega as sequências DNA e RNA em uma única sequência no processo mestre
    int local_size = local_rna_sequence.size();
    std::vector<int> recv_counts(world_size);
    MPI_Gather(&local_size, 1, MPI_INT, recv_counts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> displs(world_size, 0);
    std::string global_dna_sequence, global_rna_sequence;
    if (world_rank == 0) {
        int total_size = 0;
        for (int j = 0; j < world_size; ++j) {
            displs[j] = total_size;
            total_size += recv_counts[j];
        }
        global_dna_sequence.resize(total_size);
        global_rna_sequence.resize(total_size);
    }

    MPI_Gatherv(local_dna_sequence.data(), local_size, MPI_CHAR, &global_dna_sequence[0], recv_counts.data(), displs.data(), MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Gatherv(local_rna_sequence.data(), local_size, MPI_CHAR, &global_rna_sequence[0], recv_counts.data(), displs.data(), MPI_CHAR, 0, MPI_COMM_WORLD);

    // Escreve a sequência DNA e RNA total (limitada) no arquivo ex2/ex2_test_output.txt
    if (world_rank == 0) {
        std::ofstream output_file("ex2/ex2_limitado.txt");
        
        if (output_file.is_open()) {
            output_file << "Sequência DNA lida (limitada a " << max_test_chars << " caracteres):" << std::endl;
            output_file << global_dna_sequence << std::endl;
            output_file << "Sequência RNA transcrita:" << std::endl;
            output_file << global_rna_sequence << std::endl;
            output_file.close();
        } else {
            std::cerr << "Erro ao abrir o arquivo de saída." << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}
