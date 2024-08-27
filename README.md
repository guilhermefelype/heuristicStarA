# Documentação Detalhada do Programa de Pesquisa de Caminho com A*

## Introdução

O programa implementa o algoritmo A* (A-star) para encontrar o caminho mais curto entre dois nós em um grafo. O grafo é gerado aleatoriamente, e o programa é projetado para gerar um grafo novo e diferente a cada execução. A visualização do grafo e do caminho mais curto é realizada através de arquivos `.dot`, que são convertidos em imagens PNG utilizando a ferramenta Graphviz.

## Estruturas de Dados

### Node

A estrutura `Node` representa um nó no grafo e contém informações necessárias para o algoritmo A*.

- **id**: Identificador único do nó (um número inteiro). Utilizado para distinguir cada nó no grafo.
- **x**: Coordenada X do nó no plano 2D. Usada para calcular a heurística e para a visualização do grafo.
- **y**: Coordenada Y do nó no plano 2D. Similar ao `x`, usada para a heurística e visualização.
- **g**: Custo acumulado do nó inicial até este nó. Calculado pelo algoritmo A*.
- **h**: Heurística estimada do custo do nó até o objetivo. Calculada usando a função heurística (distância euclidiana).
- **f**: Custo total estimado, que é a soma de `g` e `h` (`f = g + h`). Utilizado para determinar a prioridade dos nós na fila de prioridade.
- **parent**: Identificador do nó predecessor no caminho mais curto. Usado para reconstruir o caminho após a execução do algoritmo.

### Edge

A estrutura `Edge` representa uma aresta que conecta dois nós no grafo.

- **from**: Identificador do nó de origem da aresta.
- **to**: Identificador do nó de destino da aresta.
- **cost**: Custo associado à aresta, que pode representar distância, tempo ou qualquer outro tipo de custo.

### Graph

A estrutura `Graph` representa o grafo completo.

- **numNodes**: Número total de nós no grafo.
- **numEdges**: Número total de arestas no grafo.
- **nodes**: Array dinâmico de `Node` que contém todos os nós do grafo.
- **edges**: Array dinâmico de `Edge` que contém todas as arestas do grafo.

## Funções

### `double heuristic(Node a, Node b)`

Calcula a heurística entre dois nós usando a distância euclidiana.

- **Parâmetros**:
  - `a`: Nó de origem.
  - `b`: Nó de destino.
- **Retorno**: Distância euclidiana entre os nós `a` e `b`, que é a raiz quadrada da soma dos quadrados das diferenças das coordenadas `x` e `y`.

### `void generate_dot(Graph* graph, const char* filename, int* path, int pathLength)`

Gera um arquivo `.dot` para visualização do grafo. O arquivo `.dot` pode ser convertido em imagens utilizando o Graphviz.

- **Parâmetros**:
  - `graph`: Ponteiro para o grafo a ser exportado.
  - `filename`: Nome do arquivo `.dot` a ser gerado.
  - `path`: Array de inteiros representando os identificadores dos nós no caminho mais curto (se aplicável).
  - `pathLength`: Comprimento do caminho mais curto (número de nós no caminho).
- **Retorno**: Nenhum.

- **Descrição**:
  - O arquivo `.dot` gerado contém a descrição do grafo em um formato que pode ser processado pelo Graphviz.
  - O grafo é descrito como não direcionado (`graph G { ... }`).
  - As arestas são adicionadas com ou sem destaque, dependendo se fazem parte do caminho mais curto.
  - Os nós são adicionados com suas coordenadas para a visualização precisa.

### `PriorityQueue* create_priority_queue(int capacity)`

Cria uma nova fila de prioridade para gerenciar os nós a serem processados pelo algoritmo A*.

- **Parâmetros**:
  - `capacity`: Capacidade inicial da fila de prioridade (número máximo de nós que a fila pode armazenar).
- **Retorno**: Ponteiro para a fila de prioridade criada.

- **Descrição**:
  - Aloca memória para a fila de prioridade e inicializa seus componentes.
  - O array `nodes` armazena os nós e o array `indices` armazena os índices dos nós na fila.

### `void insert_priority_queue(PriorityQueue* queue, Node node, int index)`

Insere um nó na fila de prioridade e ajusta a posição para manter a ordem correta.

- **Parâmetros**:
  - `queue`: Ponteiro para a fila de prioridade.
  - `node`: Nó a ser inserido.
  - `index`: Índice do nó na fila.
- **Retorno**: Nenhum.

- **Descrição**:
  - Insere um nó na fila de prioridade e ajusta a posição para garantir que o nó com o menor valor de `f` esteja no topo da fila.
  - Utiliza um heap binário para manter a ordem.

### `Node extract_min(PriorityQueue* queue)`

Extrai o nó com o menor valor de `f` da fila de prioridade.

- **Parâmetros**:
  - `queue`: Ponteiro para a fila de prioridade.
- **Retorno**: Nó com o menor valor de `f`.

- **Descrição**:
  - Remove o nó com o menor valor de `f` da fila de prioridade e reajusta a fila para manter a ordem correta.

### `void update_priority_queue(PriorityQueue* queue, Node node)`

Atualiza a prioridade de um nó na fila de prioridade.

- **Parâmetros**:
  - `queue`: Ponteiro para a fila de prioridade.
  - `node`: Nó com a nova prioridade.
- **Retorno**: Nenhum.

- **Descrição**:
  - Atualiza a posição de um nó na fila de prioridade após a alteração do valor de `f`.
  - Reajusta a fila para garantir a ordem correta.

### `void a_star(Graph* graph, int start, int goal)`

Executa o algoritmo A* para encontrar o caminho mais curto entre dois nós.

- **Parâmetros**:
  - `graph`: Ponteiro para o grafo.
  - `start`: Identificador do nó inicial.
  - `goal`: Identificador do nó objetivo.
- **Retorno**: Nenhum.

- **Descrição**:
  - Inicializa os nós e a fila de prioridade.
  - Define o custo inicial (`g`) do nó inicial como 0 e calcula o valor `f`.
  - Adiciona o nó inicial à fila de prioridade.
  - Processa nós na fila de prioridade até encontrar o nó objetivo.
  - Atualiza os custos e pais dos nós vizinhos.
  - Reconstrói o caminho mais curto a partir do nó objetivo e gera o arquivo `.dot` correspondente.
  - O caminho é representado em ordem inversa, portanto, é revertido antes de gerar o arquivo.

## Função Principal

### `int main()`

1. **Inicialização**:
   - Define o número de nós e arestas para o grafo.
   - Aloca e inicializa os nós com coordenadas aleatórias no plano 2D para garantir que os nós estejam bem distribuídos.
   - Garante que o nó inicial (0) e o nó final (49) estejam localizados em posições opostas para maximizar a distância.

2. **Geração do Grafo**:
   - Aloca e inicializa as arestas com custos aleatórios.
   - Gera o arquivo `.dot` para o grafo completo (`graph_full.dot`), representando o grafo sem considerar o caminho mais curto.

3. **Execução do Algoritmo A***:
   - Executa o algoritmo A* para encontrar o caminho mais curto entre o nó inicial e o nó final.
   - Gera o arquivo `.dot` para o grafo com o caminho mais curto destacado (`graph_shortest_path.dot`).

4. **Limpeza**:
   - Libera a memória alocada para os nós e arestas do grafo.

## Uso

1. **Compilação**:
   Compile o programa usando um compilador C. O comando a seguir compila o programa e cria o executável `pathfinding`:
   ```bash
   gcc -o pathfinding pathfinding.c -lm
   ```

2. **Execução**:
   Execute o programa para gerar o grafo e encontrar o caminho mais curto:
   ```bash
   ./pathfinding
   ```

3. **Visualização**:
   - Para o grafo completo, gere uma imagem PNG:
     ```bash
     dot -Tpng graph_full.dot -o graph_full.png
     ```
   - Para o grafo com o caminho mais curto, gere uma imagem PNG:
     ```bash
     dot -Tpng graph_shortest_path.dot -o graph_shortest_path.png
     ```
   - Visualize as imagens PNG geradas usando qualquer visualizador de imagens.

## Exemplo de Saída

- **graph_full.dot**: Representa o grafo completo com todas as arestas e custos.
- **graph_shortest_path.dot**: Representa o grafo com o caminho mais curto destacado em vermelho.

## Notas

- **Aleatoriedade**: Cada

 execução do programa gera um grafo diferente com coordenadas e custos aleatórios. Isso permite testar o algoritmo em diferentes cenários.
- **Limitações**: O número de nós e arestas é definido estaticamente no código. Para testar diferentes tamanhos de grafos, ajuste os valores de `numNodes` e `numEdges`.
