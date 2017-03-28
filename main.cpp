#include "Graph.hpp"
#include <iostream>

void doFirstTwoMovesBySymmetry(Graph &startingGraph) {
  startingGraph.colorEdge(0, 1); // Player 1 colors the first edge

  // Player 2 can either color an adjacent edge
  Graph gAdjacent(startingGraph);
  gAdjacent.colorEdge(1, 2);
  startingGraph.childGraphs.push_back(gAdjacent);

  // or a non-adjacent edge
  Graph gNonAdjacent(startingGraph);
  gNonAdjacent.colorEdge(2, 3);
  startingGraph.childGraphs.push_back(gNonAdjacent);
}

// If it's your turn, and you have a single move that will guarantee you a win,
// you have a winning strategy. If it's your turn, and no matter what move you
// make, the other player has a winning strategy, your opponent wins. If neither
// is the case, there is no winning strategy at this board.
void checkGraphHasWinner(Graph &graph) {
  int numOpponentWins = 0;
  for (auto &&g : graph.childGraphs) {
    if (g.hasWinningStrategy == graph.turn) {
      graph.hasWinningStrategy = graph.turn;
      break;
    } else if (g.hasWinningStrategy == graph.whoseTurnIsNext()) {
      ++numOpponentWins;
    }
  }
  if (numOpponentWins == graph.childGraphs.size()) {
    graph.hasWinningStrategy = graph.whoseTurnIsNext();
  }
}

void recurseAllMoves(Graph &graph, const int numVertices) {
  for (int i = 0; i < numVertices; ++i) {
    for (int j = i + 1; j < numVertices; ++j) {
      if (graph.validMove(i, j)) {
        Graph temp(graph);
        if (!temp.colorEdge(i, j)) {
          recurseAllMoves(temp, numVertices);
        }
        graph.childGraphs.push_back(temp);
      }
    }
  }
  checkGraphHasWinner(graph);
}

int main() {
  const int numVertices = 5;
  Graph startingGraph(5);
  doFirstTwoMovesBySymmetry(startingGraph);
  // startingGraph.displayGraph();
  // for (auto &&graph : startingGraph.childGraphs) {
  //   graph.displayGraph();
  // }

  for (auto &&graph : startingGraph.childGraphs) {
    recurseAllMoves(graph, numVertices);
  }

  checkGraphHasWinner(startingGraph);
  std::cout << "Who has a winning strategy?" << std::endl;
  std::cout << startingGraph.hasWinningStrategy << std::endl;
  return 0;
}
