#include <iostream>
#include <vector>

class Vector {
public:
    // Construtor que inicializa o vetor com um tamanho específico
    Vector(int tamanho) : tam(tamanho), capacidade(tamanho) {
        dados = new int[capacidade];
    }

    // Destrutor para liberar a memória alocada dinamicamente
    ~Vector() {
        delete[] dados;
    }

    // Inicializa todos os elementos do vetor com um valor específico
    void inicializa(int valor) {
        for (int i = 0; i < tam; ++i) {
            dados[i] = valor;
        }
    }

    // Acessa o valor em um índice específico (método constante)
    int get(int index) const {
        if (index < 0 || index >= tam) {
            throw std::out_of_range("Índice fora do intervalo");
        }
        // Retorna o valor no índice especificado
        std :: cout << dados[index] << std::endl;
        return dados[index];
    }

    // Modifica o valor em um índice específico
    void set(int index, int valor) {
        if (index < 0 || index >= tam) {
            throw std::out_of_range("Índice fora do intervalo");
        }
        dados[index] = valor;
    }

    // Insere um valor em um índice específico, redimensionando o vetor se necessário
    void inserir(int index, int valor) {
        if (index < 0 || index > tam) {
            throw std::out_of_range("Índice fora do intervalo");
        }
        if (tam == capacidade) {
            redimensiona(2 * capacidade);
        }
        for (int i = tam; i > index; --i) {
            dados[i] = dados[i - 1];
        }
        dados[index] = valor;
        ++tam;
    }

    // Remove o elemento de um índice específico, reduzindo o vetor se necessário
    void remover(int index) {
        if (index < 0 || index >= tam) {
            throw std::out_of_range("Índice fora do intervalo");
        }
        for (int i = index; i < tam - 1; ++i) {
            dados[i] = dados[i + 1];
        }
        --tam;
        if (tam < capacidade / 4) {
            redimensiona(capacidade / 2);
        }
    }

    // Imprime todos os elementos do vetor
    void imprime() const {
        for (int i = 0; i < tam; ++i) {
            std::cout << dados[i] << " ";
        }
        std::cout << std::endl;
    }

private:
    int* dados;
    int tam;
    int capacidade;

    // Redimensiona o vetor para uma nova capacidade
    void redimensiona(int novaCapacidade) {
        int* novo = new int[novaCapacidade];
        for (int i = 0; i < tam; ++i) {
            novo[i] = dados[i];
        }
        delete[] dados;
        dados = novo;
        capacidade = novaCapacidade;
    }
};



int main() {
    // Cria um vetor de tamanho 5 e inicializa todos os elementos com 0
    Vector vec(5);
    vec.inicializa(0);
    vec.imprime();

    // Modifica alguns valores no vetor
    vec.set(1, 10);
    vec.set(3, 20);
    vec.imprime();

    // Insere um valor no índice 2
    vec.inserir(2, 15);
    vec.imprime();

    // Remove o valor no índice 1
    vec.remover(1);
    vec.imprime();

    // Imprime o valor no índice 2
    vec.get(3);

    return 0;
}