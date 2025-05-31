#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include "helpers.cpp"

struct Vertex {
  std::string value;
  // Lista de adjacência: (destino, peso)
  std::vector<std::pair<Vertex*, int>> neighbors;
};

using nbr = std::pair<Vertex*, int>;
using pred_map = std::unordered_map<std::string, std::string>;

class Graph {
  private:
    std::vector<Vertex*> vertices;
    Vertex* findByValue(std::string value);
    void insertEdge(std::string from, std::string to, int weight);
    void printShortestPath(std::string u, std::string v, pred_map &pred);
    // Retorna todos os vértices que possuem uma aresta apontando para destination.
    std::vector<nbr> getVertexesToDestination(std::string destination);
  public:
    Graph(std::string path);
    ~Graph();
    void printVertices();
    // Executa o algoritmo de Dijkstra para encontrar caminhos mínimos a partir de start.
    // Se dest_value for passado, exibe o custo e o conjunto de arestas até o vértice de valor equivalente.
    void shortestPaths(std::string start, std::string dest_value = "");
    // Busca em largura (BFS), a partir de start, vértices tanto "para frente" (lista de adjacência)
    // quanto "para trás" (predecessores) em ordem alfabética.
    void breadthFirstSearch (std::string start);
    // Busca em profundidade (DFS), a partir de start, vértices vizinhos (lista de adjacência)
    // em ordem alfabética.
    void depthFirstSearch(std::string start);
};

Graph::Graph(std::string path) {
  std::vector<std::string> splitted_string;
  std::ifstream file(path);

  if (file.is_open()) {
    std::string line;

    while (std::getline(file, line)) {
      std::string delimeter = ";";
      splitted_string = split(line, delimeter);
      
      int weigth = 1;

      if (splitted_string.size() == 3 && !splitted_string[2].empty())
        weigth = std::stoi(splitted_string[2]);

      if (splitted_string.size() >= 2)
        insertEdge(splitted_string[0], splitted_string[1], weigth);
    }

    file.close();
  }
}

Graph::~Graph() {
  for (Vertex *v: vertices)
    delete v;

  vertices.clear();
}

void Graph::shortestPaths(std::string start, std::string dest_value) {
  pred_map pred;
  std::unordered_map<std::string, int> dist;
  
  Vertex *v0 = findByValue(start);

  if (!v0) {
    std::cout << "Valor " << start << " não encontrado.\n";
    return;
  }

  // Inicializa as distâncias com um valor muito alto.
  for (Vertex* v : vertices)
    dist[v->value] = INT_MAX;

  // Distância entre o mesmo ponto é 0.
  dist[start] = 0;

  // A partir de start: Pair (distância, valor do vértice destino)
  using Pair = std::pair<int, std::string>; 
  // Fila de prioridade com std::greater<Pair> vai ordenar as distâncias em ordem crescente.
  // De modo que pq.top() retorna o vértice com a menor distância a partir de start,
  // pois std::greater diz à fila colocar em pq.top() o elemento x tal que x < y para todo y.
  std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;
  pq.emplace(0, start);

  while (!pq.empty()) {
    auto [dist_u, u_value] = pq.top();
    pq.pop();

    if (dist_u > dist[u_value]) continue;

    Vertex *current = findByValue(u_value);

    for (nbr neighbor : current->neighbors) {
      std::string w_value = neighbor.first->value;
      int new_dist = dist_u + neighbor.second;

      // DIST[w] = min{DIST[w], DIST[u] + COST[u, v]}
      if (new_dist < dist[w_value]) {
        dist[w_value] = new_dist;
        pq.emplace(new_dist, w_value);
        // Armazena o predecessor do vértice atual.
        // Útil para imprimir o caminho de arestas.
        pred[w_value] = u_value;
      }
    }
  }

  if (!dest_value.empty() && dist[dest_value] != INT_MAX) {
    std::cout << "O menor caminho entre " << start
              << " e " << dest_value << ": " << dist[dest_value] << std::endl;

    std::cout << "O conjunto de arestas desse caminho: ";
    printShortestPath(start, dest_value, pred);
  }
}

void Graph::breadthFirstSearch(std::string start) {
  Vertex *origin = findByValue(start);

  if (!origin) {
    std::cout << "Vértice de origem não encontrado.\n";
    return;
  }

  std::queue<Vertex*> vertex_queue;
  std::unordered_set<std::string> visited_vertexes;

  std::cout << "Busca em largura partindo do vertice " << start << std::endl;
  std::cout << "Ordem de visitacao: ";

  vertex_queue.push(origin);
  visited_vertexes.insert(origin->value);

  while(!vertex_queue.empty()) {
    Vertex* current = vertex_queue.front();
    vertex_queue.pop();
    
    // Imprime o valor do vértice.
    std::cout << current->value << " ";

    std::vector<nbr> neighbors = current->neighbors;
    // Lista de vértices que têm caminho para current.
    std::vector<nbr> predecessors = getVertexesToDestination(current->value);

    neighbors.insert(neighbors.end(), predecessors.begin(), predecessors.end());

    std::sort(neighbors.begin(), neighbors.end(),
      [](nbr a, nbr b) {
        return a.first->value < b.first->value;
      }
    );
    
    for (nbr vertex : neighbors) {
      if (!visited_vertexes.count(vertex.first->value)) {
          vertex_queue.push(vertex.first);
          visited_vertexes.insert(vertex.first->value);
      }
    }
  }

  std::cout << std::endl;
}

void Graph::depthFirstSearch(std::string start) {
  std::stack<Vertex*> stack;
  std::unordered_set<std::string> visited;

  Vertex *origin = findByValue(start);

  if (!origin) {
    std::cout << "Vértice de origem não encontrado.\n";
    return;
  }

  std::cout << "Busca em profundidade partindo do vertice " << start << std::endl;
  std::cout << "Ordem de visitacao: ";

  stack.push(origin);

  while (!stack.empty()) {
    Vertex* current = stack.top();
    stack.pop();

    if (visited.count(current->value)) continue;

    visited.insert(current->value);
    // Imprime o valor do vértice.
    std::cout << current->value << " ";

    std::vector<nbr> neighbors = current->neighbors;

    // Ordena de forma decrescente para adicionar na pilha (FILO)
    std::sort(neighbors.begin(), neighbors.end(),
      [](nbr a, nbr b) {
        return a.first->value > b.first->value;
      }
    );

    // Empilha na ordem inversa para que o menor seja visitado primeiro
    for (nbr vertex : neighbors) {
      if (!visited.count(vertex.first->value)) {
        stack.push(vertex.first);
      }
    }
  }
  
  std::cout << std::endl;
}

// --- Métodos utilitários ---

void Graph::printVertices() {
  for (Vertex *v : vertices) {
    std::cout << "Vertex: " << v->value;

    for (nbr neighbor : v->neighbors) {
      std::cout << " -> " << neighbor.first->value << '(' << neighbor.second << ')';
    }

    std::cout << '\n';
  }
}

void Graph::printShortestPath(std::string u, std::string v, pred_map &pred) {
  if (v == u) return;

  printShortestPath(u, pred[v], pred);
  std::cout << "(" << pred[v] << ", " << v << ") ";
}

void Graph::insertEdge(std::string fromValue, std::string toValue, int weight) {
  // Encontra ou cria o nó de cabeçalho (from)
  Vertex* from = findByValue(fromValue);

  if (!from) {
    from = new Vertex{fromValue};
    vertices.push_back(from);
  }

  // Encontra ou cria o nó adjacente (to)
  Vertex* to = findByValue(toValue);

  if (!to) {
    to = new Vertex{toValue};
    vertices.push_back(to);
  }

  // Verifica se a aresta já existe (evita duplicata)
  for (nbr neighbor : from->neighbors)
    if (neighbor.first == to) return;

  // Adiciona o par (nó de destino, peso a partir de from)
  from->neighbors.emplace_back(to, weight);
}

Vertex* Graph::findByValue(std::string value) {
  std::vector<Vertex*>::iterator it = std::find_if(vertices.begin(), vertices.end(), 
    [&value](Vertex *v) {
      return v->value == value;
    }
  );

  if (it == vertices.end())
    return nullptr;

  return *it;
}

std::vector<nbr> Graph::getVertexesToDestination(std::string destination) {
  std::vector<nbr> result;

  for (Vertex *v : vertices) {
    for (nbr neighbor : v->neighbors) {
      if (neighbor.first->value == destination) {
        // Adicionando um par com peso 0, pois é mais barato que
        // transformar o vetor para retornar apenas a primeira componente
        // no BFS.
        result.emplace_back(v, 0);
        // Não é necessário checar os demais vizinhos de v.
        break;
      }
    }
  }

  return result;
}
