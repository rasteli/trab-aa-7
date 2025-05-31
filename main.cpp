#include <iostream>
#include "graph.cpp"

int main() {
  // (1) Leia os arquivos g1.txt, g2.txt e g3.txt e crie suas respectivas listas de
  // adjacência na memória principal.
  std::cout << "(1)";
  
  Graph g1("grafos/g1.txt");
  Graph g2("grafos/g2.txt");
  Graph g3("grafos/g3.txt");
  
  std::cout << "\nGRAFO G1\n";
  g1.printVertices();

  std::cout << "\nGRAFO G2\n";
  g2.printVertices();
  
  std::cout << "\nGRAFO G3\n";
  g3.printVertices();

  // 2) Faça a busca em largura em G1 partindo do vértice b. 
  // Exiba na tela a ordem de visitação dos vértices.
  std::cout << "\n(2)\n";
  g1.breadthFirstSearch("b");

  // 3) Faça a busca em profundidade em G2 partindo do vértice a. Exiba na
  // tela a ordem de visitação dos vértices.
  std::cout << "\n(3)\n";
  g2.depthFirstSearch("a");

  // 4) Execute o algoritmo do caminho mínimo em G3 e determine o custo
  // do menor caminho entre os vértices x e t. Exiba na tela a lista de
  // arestas correspondentes a este caminho.
  std::cout << "\n(4)\n";
  g3.shortestPaths("x", "t");

  return 0;
}
