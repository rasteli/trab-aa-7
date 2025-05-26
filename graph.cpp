#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "helpers.cpp"

using cost = std::pair<const std::string, int>;
using cost_map = std::unordered_map<std::string, int>;
using pred_map = std::unordered_map<std::string, std::string>;

struct Vertex {
  std::string value;
  Vertex *nextVertex;
  int weight;

  bool operator==(const Vertex &other) const {
    return value == other.value;
  }
};

class Graph {
  private:
    std::vector<Vertex> vertices;
    void insertVertex(Vertex v);
    void insertEdge(Vertex from, Vertex to);
    Vertex* findByValue(std::string value);
    void shortestPath(cost_map &dist, pred_map &pred, std::vector<std::string> &S);
    void printShortestPath(std::string u, std::string v, pred_map &pred);
  public:
    Graph(std::string path);
    ~Graph();
    void printVertices();
    void shortestPaths(std::string v0_value, std::string dest_value = "");
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

      Vertex from {splitted_string[0], nullptr, 0};
      Vertex to {splitted_string[1], nullptr, weigth};
      
      insertVertex(from);
      insertVertex(to);
      insertEdge(from, to);
    }

    file.close();
  }
}

Graph::~Graph() {
  for (Vertex &v : vertices) {
    Vertex *current = v.nextVertex;
    
    while (current) {
      Vertex *copy = current;
      current = current->nextVertex;
      delete copy;
    }
  }
}

void Graph::insertVertex(Vertex v) {
  std::vector<Vertex>::iterator it = std::find(vertices.begin(), vertices.end(), v);

  if (it == vertices.end())
    vertices.push_back(v);
}

void Graph::insertEdge(Vertex from, Vertex to) {
  // Encontra ou cria o nó de cabeçalho (from)
  std::vector<Vertex>::iterator it = std::find(vertices.begin(), vertices.end(), from);

  if (it == vertices.end()) {
    vertices.push_back(from);
    it = std::prev(vertices.end());
  }

  Vertex *current = &(*it);

  // Caminha até o fim da lista
  while (current->nextVertex != nullptr) {
    if (current->nextVertex->value == to.value) return; // Evita duplicatas
    current = current->nextVertex;
  }

  // Adiciona o nó de destino (to)
  current->nextVertex = new Vertex(to);
}

void Graph::shortestPaths(std::string v0_value, std::string dest_value) {
  cost_map dist;
  pred_map pred;
  
  Vertex *v0, *current;
  std::vector<std::string> S;
  
  v0 = current = findByValue(v0_value);

  if (!v0) {
    std::cout << "Valor " << v0_value << " não encontrado.\n";
    return;
  }

  for (Vertex v : vertices)
    dist[v.value] = INT_MAX;

  dist[v0_value] = 0;

  while (current->nextVertex) {
    pred[current->nextVertex->value] = v0_value;
    dist[current->nextVertex->value] = current->nextVertex->weight;
    current = current->nextVertex;
  }

  S.push_back(v0->value);

  shortestPath(dist, pred, S);

  if (!dest_value.empty() && dist[dest_value] != INT_MAX) {
    std::cout << "O menor caminho entre " << v0_value
              << " e " << dest_value << "é " << dist[dest_value] << std::endl;

    std::cout << "O conjunto de arestas desse caminho é ";
    printShortestPath(v0_value, dest_value, pred);
  }
}

void Graph::shortestPath(cost_map &dist, pred_map &pred, std::vector<std::string> &S) {
  std::string uMin;
  int distMin = INT_MAX;

  for (cost &pair : dist) {
    std::string w = pair.first;
    int dist = pair.second;

    std::vector<std::string>::iterator it = std::find(S.begin(), S.end(), w);

    // Define u tal que dist[u] = {min(dist[w])} para todo w ∉ S
    if (it == S.end() && dist < distMin) {
      uMin = w;
      distMin = dist;
    }
  }

  // Todos os vértices de G estão em S
  if (uMin.empty()) return;
  
  Vertex *u, *current;
  u = current = findByValue(uMin);

  S.push_back(current->value);
  
  while (current->nextVertex) {
    int dist_u = dist[u->value];
    int dist_w = dist[current->nextVertex->value];

    if (dist_u + current->nextVertex->weight < dist_w) {
      pred[current->nextVertex->value] = u->value;
      dist[current->nextVertex->value] = dist_u + current->nextVertex->weight;
    }

    current = current->nextVertex;
  }

  shortestPath(dist, pred, S);
}

Vertex *Graph::findByValue(std::string value) {
  Vertex match{value};
  std::vector<Vertex>::iterator it = std::find(vertices.begin(), vertices.end(), match);

  if (it == vertices.end())
    return nullptr;

  return &(*it);
}

void Graph::printVertices() {
  for (Vertex v : vertices) {
    std::cout << "Vertex: " << v.value;

    Vertex *neighbor = v.nextVertex;

    while (neighbor) {
      std::cout << " -> " << neighbor->value << '(' << neighbor->weight << ')';
      neighbor = neighbor->nextVertex;
    }

    std::cout << '\n';
  }
}

void Graph::printShortestPath(std::string u, std::string v, pred_map &pred) {
  if (v == u) return;

  printShortestPath(u, pred[v], pred);
  std::cout << "(" << pred[v] << ", " << v << ") ";
}
