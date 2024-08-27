#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// Estruturas para o grafo
typedef struct Node {
    int id;
    double x, y; // Coordenadas para a heurística
    double g; // Custo do início até o nó
    double h; // Heurística estimada
    double f; // g + h
    int parent; // Nó predecessor
} Node;

typedef struct Edge {
    int from, to;
    double cost;
} Edge;

typedef struct Graph {
    int numNodes;
    int numEdges;
    Node* nodes;
    Edge* edges;
} Graph;

// Função heurística: distância euclidiana
double heuristic(Node a, Node b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// Função para gerar um arquivo .dot do grafo
void generate_dot(Graph* graph, const char* filename, int* path, int pathLength) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "graph G {\n");
    fprintf(file, "    layout=dot;\n"); // Tente usar o layout dot para grafos grandes

    // Adiciona arestas do grafo
    for (int i = 0; i < graph->numEdges; i++) {
        Edge edge = graph->edges[i];
        bool isInPath = false;
        for (int j = 0; j < pathLength - 1; j++) {
            if ((path[j] == edge.from && path[j + 1] == edge.to) ||
                (path[j] == edge.to && path[j + 1] == edge.from)) {
                isInPath = true;
                break;
            }
        }
        if (isInPath) {
            fprintf(file, "    %d -- %d [color=red, penwidth=2.0];\n", edge.from, edge.to);
        } else {
            fprintf(file, "    %d -- %d [label=\"%.2f\"];\n", edge.from, edge.to, edge.cost);
        }
    }

    // Adiciona nós
    for (int i = 0; i < graph->numNodes; i++) {
        fprintf(file, "    %d [label=\"%d\", pos=\"%.2f,%.2f!\"];\n",
                graph->nodes[i].id, graph->nodes[i].id, graph->nodes[i].x, graph->nodes[i].y);
    }

    fprintf(file, "}\n");
    fclose(file);
}

typedef struct PriorityQueue {
    Node* nodes;
    int* indices;
    int size;
} PriorityQueue;

PriorityQueue* create_priority_queue(int capacity) {
    PriorityQueue* queue = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    queue->nodes = (Node*)malloc(capacity * sizeof(Node));
    queue->indices = (int*)malloc(capacity * sizeof(int));
    queue->size = 0;
    memset(queue->indices, -1, capacity * sizeof(int));
    return queue;
}

void insert_priority_queue(PriorityQueue* queue, Node node, int index) {
    int i = queue->size++;
    queue->nodes[i] = node;
    queue->indices[index] = i;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (queue->nodes[i].f >= queue->nodes[parent].f)
            break;
        Node temp = queue->nodes[i];
        queue->nodes[i] = queue->nodes[parent];
        queue->indices[queue->nodes[i].id] = i;
        queue->nodes[parent] = temp;
        queue->indices[queue->nodes[parent].id] = parent;
        i = parent;
    }
}

Node extract_min(PriorityQueue* queue) {
    Node min = queue->nodes[0];
    queue->size--;
    queue->nodes[0] = queue->nodes[queue->size];
    queue->indices[queue->nodes[0].id] = 0;

    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left < queue->size && queue->nodes[left].f < queue->nodes[smallest].f)
            smallest = left;
        if (right < queue->size && queue->nodes[right].f < queue->nodes[smallest].f)
            smallest = right;
        if (smallest == i)
            break;
        Node temp = queue->nodes[i];
        queue->nodes[i] = queue->nodes[smallest];
        queue->indices[queue->nodes[i].id] = i;
        queue->nodes[smallest] = temp;
        queue->indices[queue->nodes[smallest].id] = smallest;
        i = smallest;
    }
    return min;
}

void update_priority_queue(PriorityQueue* queue, Node node) {
    int i = queue->indices[node.id];
    queue->nodes[i] = node;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (queue->nodes[i].f >= queue->nodes[parent].f)
            break;
        Node temp = queue->nodes[i];
        queue->nodes[i] = queue->nodes[parent];
        queue->indices[queue->nodes[i].id] = i;
        queue->nodes[parent] = temp;
        queue->indices[queue->nodes[parent].id] = parent;
        i = parent;
    }
}

void a_star(Graph* graph, int start, int goal) {
    Node* nodes = malloc(graph->numNodes * sizeof(Node));
    int* cameFrom = malloc(graph->numNodes * sizeof(int));
    PriorityQueue* openSet = create_priority_queue(graph->numNodes);

    for (int i = 0; i < graph->numNodes; i++) {
        nodes[i].id = i;
        nodes[i].g = DBL_MAX;
        nodes[i].h = heuristic(graph->nodes[i], graph->nodes[goal]);
        nodes[i].f = DBL_MAX;
        nodes[i].parent = -1;
        cameFrom[i] = -1;
    }

    nodes[start].g = 0;
    nodes[start].f = nodes[start].h;
    insert_priority_queue(openSet, nodes[start], start);

    while (openSet->size > 0) {
        Node current = extract_min(openSet);
        if (current.id == goal) {
            break;
        }

        for (int i = 0; i < graph->numEdges; i++) {
            Edge edge = graph->edges[i];
            if (edge.from == current.id || edge.to == current.id) {
                int neighbor = (edge.from == current.id) ? edge.to : edge.from;
                double tentative_g = current.g + edge.cost;
                if (tentative_g < nodes[neighbor].g) {
                    nodes[neighbor].parent = current.id;
                    nodes[neighbor].g = tentative_g;
                    nodes[neighbor].f = nodes[neighbor].g + nodes[neighbor].h;
                    if (openSet->indices[neighbor] == -1) {
                        insert_priority_queue(openSet, nodes[neighbor], neighbor);
                    } else {
                        update_priority_queue(openSet, nodes[neighbor]);
                    }
                }
            }
        }
    }

    // Reconstituir o caminho
    int path[graph->numNodes];
    int pathLength = 0;
    for (int at = goal; at != -1; at = nodes[at].parent) {
        path[pathLength++] = at;
    }
    for (int i = 0; i < pathLength / 2; i++) {
        int temp = path[i];
        path[i] = path[pathLength - 1 - i];
        path[pathLength - 1 - i] = temp;
    }

    // Gerar grafo com caminho mais curto
    generate_dot(graph, "graph_shortest_path.dot", path, pathLength);

    free(nodes);
    free(cameFrom);
    free(openSet->nodes);
    free(openSet->indices);
    free(openSet);
}

int main() {
    // Semente para o gerador de números aleatórios
    srand(time(NULL));

    // Definição do grafo
    Graph graph;
    graph.numNodes = 50; // Número de nós
    graph.numEdges = 200; // Número de arestas (exemplo, ajuste conforme necessário)

    // Alocar e inicializar nós
    graph.nodes = (Node*)malloc(graph.numNodes * sizeof(Node));
    for (int i = 0; i < graph.numNodes; i++) {
        graph.nodes[i].id = i;
        graph.nodes[i].x = (rand() % 2000) - 1000; // Coordenadas aleatórias para maior distância
        graph.nodes[i].y = (rand() % 2000) - 1000;
    }

    // Garantir que o nó inicial e final estejam bem distantes
    graph.nodes[0].x = -1000;
    graph.nodes[0].y = -1000;
    graph.nodes[49].x = 1000;
    graph.nodes[49].y = 1000;

    // Alocar e inicializar arestas (exemplo, ajuste conforme necessário)
    graph.edges = (Edge*)malloc(graph.numEdges * sizeof(Edge));
    for (int i = 0; i < graph.numEdges; i++) {
        graph.edges[i].from = rand() % graph.numNodes;
        graph.edges[i].to = rand() % graph.numNodes;
        graph.edges[i].cost = (rand() % 100) + 1; // Custo aleatório
        // Evitar arestas de um nó para ele mesmo
        if (graph.edges[i].from == graph.edges[i].to) {
            graph.edges[i].to = (graph.edges[i].to + 1) % graph.numNodes;
        }
    }

    // Garantir que o grafo é conectado (opcional, dependendo da necessidade)
    // Implementar uma verificação ou ajustar a geração de arestas para garantir conectividade

    // Gerar grafo completo
    generate_dot(&graph, "graph_full.dot", NULL, 0);

    // Encontrar o menor caminho
    a_star(&graph, 0, 49); // Exemplo: do nó 0 ao nó 49

    // Limpar
    free(graph.nodes);
    free(graph.edges);

    return 0;
}
