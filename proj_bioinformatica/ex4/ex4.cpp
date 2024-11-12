#include <iostream>
#include <fstream>
#include <vector>
#include <mpi.h>
#include <omp.h>
#include <string>
#include <unordered_map>
#include <filesystem>

namespace fs = std::filesystem;

// Mapeamento de códons para os números correspondentes dos aminoácidos
std::unordered_map<std::string, int> codon_to_amino_acid = {
    {"CCA", 1}, {"CCG", 1}, {"CCU", 1}, {"CCC", 1},       // Prolina
    {"UCU", 2}, {"UCA", 2}, {"UCG", 2}, {"UCC", 2},       // Serina
    {"CAG", 3}, {"CAA", 3},                               // Glutamina
    {"ACA", 4}, {"ACC", 4}, {"ACU", 4}, {"ACG", 4},       // Treonina
    {"AUG", 5},                                           // Metionina (início)
    {"UGA", 0},                                           // Códon STOP
    {"UGC", 6}, {"UGU", 6},                               // Cisteína
    {"GUG", 7}, {"GUA", 7}, {"GUC", 7}, {"GUU", 7}        // Valina
};

// Função para traduzir uma sequência de RNA para uma sequência de números representando aminoácidos
std::vector<int> translateRNAtoAminoAcids(const std::string& rna_sequence) {
    std::vector<int> amino_acid_sequence;
    bool stop_found = false;

    #pragma omp parallel
    {
        std::vector<int> local_amino_acid_sequence;

        #pragma omp for
        for (size_t i = 0; i <= rna_sequence.size() - 3; i += 3) {
            if (stop_found) continue;  // Ignora o restante após encontrar o STOP

            std::string codon = rna_sequence.substr(i, 3);
            if (codon_to_amino_acid.count(codon) > 0) {
                int amino_acid = codon_to_amino_acid[codon];
                if (amino_acid == 0) {  // Códon STOP
                    #pragma omp atomic write
                    stop_found = true;
                } else {
                    local_amino_acid_sequence.push_back(amino_acid);
                }
            }
        }

        #pragma omp critical
        amino_acid_sequence.insert(amino_acid_sequence.end(), local_amino_acid_sequence.begin(), local_amino_acid_sequence.end());
    }

    return amino_acid_sequence;
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

    std::string input_folder = "ex2/outputs";
    std::string output_folder = "ex4/outputs";
    if (world_rank == 0) {
        fs::create_directories(output_folder);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    for (int i = 1; i <= 22; i++) {
        std::string filename = input_folder + "/chr" + std::to_string(i) + "_rna.txt";
        std::string output_filename = output_folder + "/chr" + std::to_string(i) + "_amino_acids.txt";

        std::string rna_sequence;
        if (world_rank == 0) {
            rna_sequence = readRNAFile(filename);
        }

        int chunk_size = rna_sequence.size() / world_size;
        std::string local_rna_sequence = rna_sequence.substr(world_rank * chunk_size, chunk_size);

        if (world_rank == world_size - 1) {
            local_rna_sequence += rna_sequence.substr(world_rank * chunk_size + chunk_size);
        }

        std::vector<int> local_amino_acid_sequence = translateRNAtoAminoAcids(local_rna_sequence);

        int local_size = local_amino_acid_sequence.size();
        std::vector<int> recv_counts(world_size);
        MPI_Gather(&local_size, 1, MPI_INT, recv_counts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

        std::vector<int> displs(world_size, 0);
        int total_size = 0;
        if (world_rank == 0) {
            for (int j = 0; j < world_size; ++j) {
                displs[j] = total_size;
                total_size += recv_counts[j];
            }
        }

        std::vector<int> amino_acid_sequence;
        if (world_rank == 0) {
            amino_acid_sequence.resize(total_size);
        }

        MPI_Gatherv(local_amino_acid_sequence.data(), local_size, MPI_INT, amino_acid_sequence.data(), recv_counts.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);

        if (world_rank == 0) {
            std::ofstream output_file(output_filename);
            if (output_file.is_open()) {
                output_file << "Sequência de aminoácidos para chr" << i << ":" << std::endl;
                for (int amino_acid : amino_acid_sequence) {
                    output_file << amino_acid << " ";
                }
                output_file << std::endl;
                output_file.close();
            } else {
                std::cerr << "Erro ao abrir o arquivo de saída: " << output_filename << std::endl;
            }
        }
    }

    MPI_Finalize();
    return 0;
}
