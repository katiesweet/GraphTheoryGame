#include <fstream>
#include <string>
#include <vector>

class Graph {
public:
  Graph(int numVertices);
  void displayGraph(std::string fileName);
  bool validMove(int i, int j);
  bool colorEdge(int i, int j);
  int whoseTurnIsNext();
  int turn;
  int hasWinningStrategy;
  std::vector<std::shared_ptr<Graph>> childGraphs;

private:
  std::vector<int> vertices;
  std::vector<std::vector<std::string>> adjacencyMatrix;

  bool isTriangle(int i, int j, int k);
  bool checkGameOver();
  std::string getColor();
};
