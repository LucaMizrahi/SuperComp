### Perguntas para Reflexão:

1. **Qual abordagem foi mais rápida?**
   - O tempo de execução pode variar dependendo do compilador e da máquina. Geralmente, `std::vector` pode ser um pouco mais lento na alocação inicial devido à segurança adicional que oferece, mas a diferença tende a ser pequena No caso do códigos realizados, a diferenças foi apenas 0.003 segundos. A vantagem do `std::vector` se destaca na facilidade de uso e segurança, o que justamente pode ser observado pelo fato do código ser muito mais simples e menos propenso a erros.

2. **Quais vantagens você percebe ao usar `std::vector`?**
   - `std::vector` gerencia automaticamente a alocação e liberação de memória, reduzindo o risco de vazamentos de memória.
   - O código é mais conciso e menos propenso a erros, especialmente em situações complexas de gerenciamento de memória.
   - `std::vector` oferece funções adicionais que facilitam operações comuns em arrays dinâmicos, como redimensionamento e verificação de limites.

3. **Como a alocação automática e a liberação de memória pelo `std::vector` afetam a segurança e robustez do código?**
   - Ao eliminar a necessidade de `delete[]`, `std::vector` previne erros comuns como vazamentos de memória e dangling pointers, tornando o código mais robusto e seguro.
   - `std::vector` também garante que a memória seja liberada automaticamente quando o objeto sai do escopo, simplificando o gerenciamento de recursos e evitando erros relacionados à liberação de memória.
