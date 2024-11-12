#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

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
    
    for (size_t i = 0; i <= rna_sequence.size() - 3; i += 3) {
        std::string codon = rna_sequence.substr(i, 3);
        if (codon_to_amino_acid.count(codon) > 0) {
            int amino_acid = codon_to_amino_acid[codon];
            if (amino_acid == 0) {  // Códon STOP
                break;
            }
            amino_acid_sequence.push_back(amino_acid);
        }
    }
    
    return amino_acid_sequence;
}

// Função para ler o conteúdo de um arquivo RNA de teste
std::string readTestRNAFile(const std::string& filename) {
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

int main() {
    // Lê a sequência de RNA do arquivo de teste
    std::string rna_sequence = readTestRNAFile("ex4_test_rna_v2.txt");

    // Traduz a sequência de RNA para uma sequência de aminoácidos
    std::vector<int> amino_acid_sequence = translateRNAtoAminoAcids(rna_sequence);

    // Escreve a sequência de aminoácidos em um arquivo de saída de teste
    std::ofstream output_file("ex4_teste.txt");
    if (output_file.is_open()) {
        output_file << "Sequência de aminoácidos para o arquivo de teste:" << std::endl;
        for (int amino_acid : amino_acid_sequence) {
            output_file << amino_acid << " ";
        }
        output_file << std::endl;
        output_file.close();
        std::cout << "Saída escrita em ex4_teste.txt" << std::endl;
    } else {
        std::cerr << "Erro ao abrir o arquivo de saída ex4_teste.txt" << std::endl;
    }

    return 0;
}
