#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <algorithm> 

struct Vertex {
  std::string value;
  Vertex* nextVertex;
  int weight;

  bool operator==(const Vertex &other) const {
    return value == other.value;
  }
};

std::vector<std::string> split(std::string &s, std::string &delimiter);
void insertEdge(std::list<Vertex> &graph, Vertex from, Vertex to);
void buildListFromFile(std::list<Vertex> &graph, std::string path);
void printVertices(std::list<Vertex> &vertices);

int main() {
  std::list<Vertex> g1, g2, g3;

  buildListFromFile(g1, "grafos/g1.txt");
  buildListFromFile(g2, "grafos/g2.txt");
  buildListFromFile(g3, "grafos/g3.txt");
  
  std::cout << "\nGRAFO G1\n";
  printVertices(g1);

  std::cout << "\nGRAFO G2\n";
  printVertices(g2);
  
  std::cout << "\nGRAFO G3\n";
  printVertices(g3);
  return 0;
}

std::vector<std::string> split(std::string &s, std::string &delimiter) {
  std::vector<std::string> tokens;
  size_t pos = 0;
  std::string token;

  while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    tokens.push_back(token);
    s = s.substr(pos + delimiter.length());
  }

  return tokens;
}

void insertEdge(std::list<Vertex> &graph, Vertex from, Vertex to) {
  // Encontra ou cria o nó de cabeçalho (from)
  std::list<Vertex>::iterator it = std::find(graph.begin(), graph.end(), from);

  if (it == graph.end()) {
    graph.push_back(from);
    it = std::prev(graph.end());
  }

  Vertex* current = &(*it);

  // Caminha até o fim da lista
  while (current->nextVertex != nullptr) {
    if (current->nextVertex->value == to.value) return; // Evita duplicatas
    current = current->nextVertex;
  }

  // Adiciona o nó de destino (to)
  current->nextVertex = new Vertex(to);
}

void buildListFromFile(std::list<Vertex> &graph, std::string path) {
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

      Vertex from {splitted_string[0], nullptr, weigth};
      Vertex to {splitted_string[1], nullptr, weigth};
      
      insertEdge(graph, from, to);
    }

    file.close();
  }
}

void printVertices(std::list<Vertex> &vertices) {
  for (Vertex v : vertices) {
    std::cout << "Vertex: " << v.value;

    Vertex* neighbor = v.nextVertex;
    if (neighbor) std::cout << " -> ";

    while (neighbor != nullptr) {
      std::cout << neighbor->value << '(' << neighbor->weight << ')';
      neighbor = neighbor->nextVertex;
      if (neighbor) std::cout << " -> ";
    }

    std::cout << '\n';
  }
}
