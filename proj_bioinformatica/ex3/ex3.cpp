#include <iostream>
#include <fstream>
#include <vector>
#include <mpi.h>
#include <omp.h>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

// Função para contar proteínas inicializadas em uma sequência de RNA
int countProteinsInRNA(const std::string& rna_sequence) {
    int protein_count = 0;
    bool in_protein = false;

    #pragma omp parallel for reduction(+:protein_count)
    for (size_t i = 0; i <= rna_sequence.size() - 3; i += 3) { // Processa por códons
        std::string codon = rna_sequence.substr(i, 3);

        if (codon == "AUG" && !in_protein) {
            in_protein = true;  // Início de uma nova proteína
            protein_count++;
        } else if (in_protein && (codon == "UAA" || codon == "UAG" || codon == "UGA")) {
            in_protein = false; // Finaliza a proteína atual
        }
    }

    return protein_count;
}

// Função para ler o conteúdo de um arquivo RNA
std::string readRNAFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line, sequence;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            sequence += line;
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

    // Define a pasta onde os arquivos de RNA estão localizados
    std::string input_folder = "ex2/outputs";
    int total_proteins = 0;

    // Arquivo de saída para as contagens de proteínas
    std::ofstream output_file;
    if (world_rank == 0) {
        output_file.open("ex3/ex3.txt");
        if (output_file.is_open()) {
            output_file << "Contagem de proteínas inicializadas em cada arquivo de RNA:" << std::endl;
        } else {
            std::cerr << "Erro ao abrir o arquivo de saída para contagem de proteínas." << std::endl;
        }
    }

    // Itera sobre os arquivos de RNA (chr1_rna.txt a chr22_rna.txt)
    for (int i = 1; i <= 22; i++) {
        std::string filename = input_folder + "/chr" + std::to_string(i) + "_rna.txt";

        // O processo mestre lê o arquivo de RNA e distribui a sequência entre os processos
        std::string rna_sequence;
        if (world_rank == 0) {
            rna_sequence = readRNAFile(filename);
        }

        // Divide a sequência entre os processos
        int chunk_size = rna_sequence.size() / world_size;
        std::string local_rna_sequence = rna_sequence.substr(world_rank * chunk_size, chunk_size);

        // O último processo pega qualquer caractere restante
        if (world_rank == world_size - 1) {
            local_rna_sequence += rna_sequence.substr(world_rank * chunk_size + chunk_size);
        }

        // Conta as proteínas na sequência local
        int local_protein_count = countProteinsInRNA(local_rna_sequence);

        // Agrega as contagens de proteínas de todos os processos
        int protein_count_in_file = 0;
        MPI_Reduce(&local_protein_count, &protein_count_in_file, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        // Acumula o total de proteínas no processo mestre e registra no arquivo de saída
        if (world_rank == 0) {
            total_proteins += protein_count_in_file;
            if (output_file.is_open()) {
                output_file << "Proteínas encontradas em chr" << i << ": " << protein_count_in_file << std::endl;
            }
        }
    }

    // Registra o total de proteínas encontradas em todos os arquivos no arquivo de saída
    if (world_rank == 0) {
        if (output_file.is_open()) {
            output_file << "Total de proteínas encontradas em todos os arquivos: " << total_proteins << std::endl;
            output_file.close();
        }
    }

    MPI_Finalize();
    return 0;
}
