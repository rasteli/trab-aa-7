#include <iostream>
#include "graph.cpp"

int main() {
  std::cout << "(1)";
  // (1) Leia os arquivos g1.txt, g2.txt e g3.txt e crie suas respectivas listas de
  // adjacência na memória principal.
  Graph g1("grafos/g1.txt");
  Graph g2("grafos/g2.txt");
  Graph g3("grafos/g3.txt");
  
  std::cout << "\nGRAFO G1\n";
  g1.printVertices();

  std::cout << "\nGRAFO G2\n";
  g2.printVertices();
  
  std::cout << "\nGRAFO G3\n";
  g3.printVertices();

  cout << "\n(2)\n";
  // 2) Faça a busca em largura em G1 partindo do vértice b. 
  // Exiba na tela a ordem de visitação dos vértices. 
  g1.breadthFirstSearch("b");

  std::cout << "\n(4)\n";
  // 4) Execute o algoritmo do caminho mínimo em G3 e determine o custo
  // do menor caminho entre os vértices x e t. Exiba na tela a lista de
  // arestas correspondentes a este caminho.
  g3.shortestPaths("x", "t");

  return 0;
}
