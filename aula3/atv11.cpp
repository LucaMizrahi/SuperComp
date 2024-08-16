#include <iostream>
#include <fstream>
#include <map>
#include <vector>

int main() {
    std::string filename;
    std::cout << "Digite o nome do arquivo: ";
    std::cin >> filename;

    std::ifstream file(filename
    );
    if (!file.is_open()) {
        std::cout << "Erro ao abrir o arquivo" << std::endl;
        return 1;
    }
    
    std::map<std::string, int> word_count;
    int total_words = 0;
    int total_lines = 0;
    std::string line;
    while (std::getline(file, line)) {
        total_lines++;
        std::string word;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ' ' || line[i] == '\n' || line[i] == '\t') {
                if (word.size() > 0) {
                    word_count[word]++;
                    total_words++;
                    word = "";
                }
            } else {
                word += line[i];
            }
        }
        if (word.size() > 0) {
            word_count[word]++;
            total_words++;
        }
    }

    std::string most_frequent_word;
    int most_frequent_word_count = 0;
    for (auto it = word_count.begin(); it != word_count.end(); it++) {
        if (it->second > most_frequent_word_count) {
            most_frequent_word = it->first;
            most_frequent_word_count = it->second;
        }
    }

    std::ofstream output("output_11.txt");
    output << "Número total de palavras: " << total_words << std::endl;
    output << "Número total de linhas: " << total_lines << std::endl;
    output << "Número médio de palavras por linha: " << (double) total_words / total_lines << std::endl;
    output << "Palavra mais frequente: " << most_frequent_word << " (" << most_frequent_word_count << " ocorrências)" << std::endl;
    output.close();

    return 0;
} 

// Para compilar: g++ atv11.cpp -o atv11
// Para executar: srun --ntasks=1 --cpus-per-task=1 --mem=500M ./atv11