#include "Graph.hpp"

Graph::Graph(int numVertices) : turn(1), hasWinningStrategy(0) {
  adjacencyMatrix.resize(numVertices);
  for (auto &&vector : adjacencyMatrix) {
    vector.resize(numVertices);
  }
  for (int i = 0; i < numVertices; ++i) {
    vertices.push_back(i);
    for (int j = i + 1; j < numVertices; ++j) {
      adjacencyMatrix[i][j] = std::string("grey");
      adjacencyMatrix[j][i] = std::string("grey");
    }
  }
}

void Graph::displayGraph(std::string fileName) {
  // std::string fileName = "graph" + std::to_string(counter()) + ".dot";
  std::ofstream file(fileName);
  file << "strict graph {" << std::endl;
  for (int i = 0; i < vertices.size(); ++i) {
    for (int j = i + 1; j < vertices.size(); ++j) {
      file << i << "--" << j << " [color=" << adjacencyMatrix[i][j] << "];"
           << std::endl;
    }
  }
  file << "}" << std::endl;
  std::string syscall =
      "circo -Tpng -O " + fileName + " && open " + fileName + ".png ";
  system(syscall.c_str());
  file.close();
}

bool Graph::isTriangle(int i, int j, int k) {
  if (adjacencyMatrix[i][j] == adjacencyMatrix[j][k] &&
      adjacencyMatrix[i][j] == adjacencyMatrix[i][k] &&
      adjacencyMatrix[i][j] != std::string("grey")) {
    return true;
  }
  return false;
}

bool Graph::checkGameOver() {
  // Check all possible K_3's
  for (int i = 0; i < vertices.size(); ++i) {
    for (int j = i + 1; j < vertices.size(); ++j) {
      for (int k = j + 1; k < vertices.size(); ++k) {
        if (isTriangle(i, j, k)) {
          if (adjacencyMatrix[i][j] == std::string("red")) {
            hasWinningStrategy = 2;
          } else {
            hasWinningStrategy = 1;
          }
          // displayGraph();
          return true;
        }
      }
    }
  }
  return false;
}

bool Graph::validMove(int i, int j) {
  if (adjacencyMatrix[i][j] == std::string("grey")) {
    return true;
  }
  return false;
}

bool Graph::colorEdge(int i, int j) {
  if (validMove(i, j)) {
    adjacencyMatrix[i][j] = getColor();
  } else {
    throw std::runtime_error(
        "Cannot color an edge that has already been colored");
  }
  turn = whoseTurnIsNext();
  return checkGameOver();
}

std::string Graph::getColor() {
  if (turn == 1) {
    return std::string("red");
  } else {
    return std::string("blue");
  }
}

int Graph::whoseTurnIsNext() {
  if (turn == 1) {
    return 2;
  } else {
    return 1;
  }
}
