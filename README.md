<h1 align='center'> Trabalho Prático ED 2 </h1>

<p align='center'>
<img alt="C" src="https://img.shields.io/badge/C-555555?style=flat&logo=c&logoColor=white">
</p>

<p align="center">
    <b>Edson Takashi Matsubara | Estruturas de Dados</b>
</p>

## 📚 Descrição

O desafio consistia basicamente em implementar uma tabela hash e 5 árvores AVL, dividido em 4 tarefas:

### Tarefa 1: (adaptação da AVL)

Na tarefa 1, é necessário fazer algumas adaptações na AVL, então ela é dividida em 3 sub-tarefas:

- Adaptar o código de AVL para que os nós possuam um ponteiro para o pai.
- Implementar uma lista encadeada para cada nó da árvore que armazena os registros com chaves iguais (um nó pode ter dois ou mais registros).
- Implementar a função de sucessor de um nó que consiga olhar para os ancestrais, o que permite fazer o range query.

### Tarefa 2 (construção das avls)

Construa uma AVL para cada um dos seguintes campos como chave da busca: "nome", "latitude", "longitude","codigo_uf" e "ddd". Deste modo você terá que instanciar cinco AVLs, uma para cada campo. O registro armazenado deve conter a chave de busca + o código_ibge. Observe que na AVL adaptada as chaves replicadas são armazenadas um mesmo nó. Por exemplo todas as cidades com DDD 67 serão amazenadas em um mesmo nó.

### Tarefa 3 (combinação de range queries).

Considere as seguintes três queries: (1) cidades com latitude > 50, (2) 20 <longitude < 30 e (3) DDD == 67. Cada query irá retornar um conjunto de códigos ibge. A combinação consiste em fazer a intersecção desses conjuntos.

### Tarefa 4 (interface)

Faça uma interface para que seja possível fazer range queries e combinação de range queries com qualquer um dos cinco campos definidos na tarefa 2. A busca deve retornar todos do campos das cidades fornecidas na base de dados. Para a busca das cidades pelo código IBGE você deve utilizar uma tabela hash.

## 🚀 Funcionalidades

- [x] Tarefa 1 - AVL adapatada com ponteiro para o pai, lista encadeada e função de sucessor
- [x] Tarefa 2 - Construção das 5 AVLs e inserção dos registros
- [x] Tarefa 3 - Possibilidade de fazer range queries e combinação de range queries com os campos "nome", "latitude", "longitude", "codigo_uf" e "ddd"
- [x] Tarefa 4 - Interface funcional para realizar as buscas e uso de tabela hash para pegar as informações da cidade pelo código IBGE

## 📝 Como rodar

Para compilar e executar o programa, execute o comando abaixo:

```bash
./run.sh
```
