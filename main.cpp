#include "Graph.hpp"
#include <iostream>

std::string counter() {
  static int i = 0;
  ++i;
  return std::to_string(i);
  // return ++i;
}

void adjacentThirdMove(std::vector<std::shared_ptr<Graph>> &fourthTurnGraphs,
                       std::shared_ptr<Graph> startingChild1) {
  // Case 1: Tree with two red edges and one blue edge meeting at one vertex
  auto case1 = std::make_shared<Graph>(*startingChild1);
  case1->childGraphs.clear();
  case1->colorEdge(1, 3);
  fourthTurnGraphs.push_back(case1);
  startingChild1->childGraphs.push_back(case1);

  // Case 2: Path with red edge, red edge, blue edge
  auto case2 = std::make_shared<Graph>(*startingChild1);
  case2->childGraphs.clear();
  case2->colorEdge(0, 4);
  fourthTurnGraphs.push_back(case2);
  startingChild1->childGraphs.push_back(case2);

  // Case 3: Path with red ege, blue edge, red edge
  auto case3 = std::make_shared<Graph>(*startingChild1);
  case3->childGraphs.clear();
  case3->colorEdge(2, 3);
  fourthTurnGraphs.push_back(case3);
  startingChild1->childGraphs.push_back(case3);

  // Case 4: K3 with two red edges and a blue edge
  auto case4 = std::make_shared<Graph>(*startingChild1);
  case4->childGraphs.clear();
  case4->colorEdge(0, 2);
  fourthTurnGraphs.push_back(case4);
  startingChild1->childGraphs.push_back(case4);

  // Case 5: One path with both a red and blue edge, one unconnected red K2
  auto case5 = std::make_shared<Graph>(*startingChild1);
  case5->childGraphs.clear();
  case5->colorEdge(3, 4);
  fourthTurnGraphs.push_back(case5);
  startingChild1->childGraphs.push_back(case5);
}

void nonAdjacentFirstMove(std::vector<std::shared_ptr<Graph>> &fourthTurnGraphs,
                          std::shared_ptr<Graph> startingChild2) {
  // Case 1: One path with two red edges, one unconnected blue K2
  auto case1 = std::make_shared<Graph>(*startingChild2);
  case1->childGraphs.clear();
  case1->colorEdge(0, 3);
  fourthTurnGraphs.push_back(case1);
  startingChild2->childGraphs.push_back(case1);

  // Case 2: Path with red edge, red edge, blue edge
  auto case2 = std::make_shared<Graph>(*startingChild2);
  case2->childGraphs.clear();
  case2->colorEdge(1, 2);
  fourthTurnGraphs.push_back(case2);
  startingChild2->childGraphs.push_back(case2);

  // Case 3: One path with both a red and blue edge, one unconnected red K2
  auto case3 = std::make_shared<Graph>(*startingChild2);
  case3->childGraphs.clear();
  case3->colorEdge(3, 4);
  fourthTurnGraphs.push_back(case3);
  startingChild2->childGraphs.push_back(case3);
}

std::vector<std::shared_ptr<Graph>>
thirdMove(std::shared_ptr<Graph> startingChild1,
          std::shared_ptr<Graph> startingChild2) {
  std::vector<std::shared_ptr<Graph>> fourthTurnGraphs;
  adjacentThirdMove(fourthTurnGraphs, startingChild1);
  nonAdjacentFirstMove(fourthTurnGraphs, startingChild2);
  return fourthTurnGraphs;
}

std::vector<std::shared_ptr<Graph>>
doFirstThreeMoves(std::shared_ptr<Graph> startingGraph) {
  startingGraph->colorEdge(0, 1); // Player 1 colors the first edge

  // Player 2 can either color an adjacent edge
  auto gAdjacent = std::make_shared<Graph>(*startingGraph);
  gAdjacent->childGraphs.clear();
  gAdjacent->colorEdge(1, 2);
  startingGraph->childGraphs.push_back(gAdjacent);

  // or a non-adjacent edge
  auto gNonAdjacent = std::make_shared<Graph>(*startingGraph);
  gNonAdjacent->childGraphs.clear();
  gNonAdjacent->colorEdge(2, 4);
  startingGraph->childGraphs.push_back(gNonAdjacent);

  return thirdMove(gAdjacent, gNonAdjacent);
}

// If it's your turn, and you have a single move that will guarantee you a win,
// you have a winning strategy. If it's your turn, and no matter what move you
// make, the other player has a winning strategy, your opponent wins. If neither
// is the case, there is no winning strategy at this board.
void checkGraphHasWinner(std::shared_ptr<Graph> graph) {
  int numOpponentWins = 0;
  for (auto &&g : graph->childGraphs) {
    if (g->hasWinningStrategy == graph->turn) {
      graph->hasWinningStrategy = graph->turn;
      break;
    } else if (g->hasWinningStrategy == graph->whoseTurnIsNext()) {
      ++numOpponentWins;
    }
  }
  if (numOpponentWins == graph->childGraphs.size()) {
    graph->hasWinningStrategy = graph->whoseTurnIsNext();
  }
}

// For each valid move remaining in the graph, make that move and recurse into
// those graphs
void recurseAllMoves(std::shared_ptr<Graph> graph, const int numVertices) {
  for (int i = 0; i < numVertices; ++i) {
    for (int j = i + 1; j < numVertices; ++j) {
      if (graph->validMove(i, j)) {
        auto temp = std::make_shared<Graph>(*graph);
        temp->childGraphs.clear();
        if (!temp->colorEdge(i, j)) {
          recurseAllMoves(temp, numVertices);
        }
        graph->childGraphs.push_back(temp);
      }
    }
  }
  checkGraphHasWinner(graph);
}

void printOneStrategy(std::shared_ptr<Graph> graph) {
  for (auto &&g : graph->childGraphs) {
    if (g->hasWinningStrategy == graph->hasWinningStrategy) {
      g->displayGraph("graph" + counter());
      printOneStrategy(g);
      break;
    }
  }
}

void printWinningStrategy(std::shared_ptr<Graph> graph) {
  // for each level in tree:
  // if it is red's turn, print all of red's possible moves
  // if it is blue's turn, print a winning strategy move
  std::string graphNumber = counter();
  int counter = 0;
  for (auto &&g : graph->childGraphs) {
    if (graph->turn == 2 && g->hasWinningStrategy == 2) {
      g->displayGraph("bluesMove" + graphNumber + ".dot");
      printWinningStrategy(g);
      break;
    } else if (graph->turn == 1) {
      g->displayGraph("redsMove" + graphNumber + "_" +
                      std::to_string(++counter) + ".dot");
      printWinningStrategy(g);
    }
  }
}

int main() {
  const int numVertices = 5;
  auto startingGraph = std::make_shared<Graph>(numVertices);
  // doFirstTwoMovesBySymmetry(startingGraph);

  auto fourthTurnGraphs = doFirstThreeMoves(startingGraph);
  for (auto &&graph : fourthTurnGraphs) {
    recurseAllMoves(graph, numVertices);
  }

  for (auto &&graph : startingGraph->childGraphs) {
    // recurseAllMoves(graph, numVertices);
    checkGraphHasWinner(graph);
  }

  checkGraphHasWinner(startingGraph);
  std::cout << "Who has a winning strategy?" << std::endl;
  std::cout << startingGraph->hasWinningStrategy << std::endl;

  // printWinningStrategy(startingGraph);

  std::cout << "Who wins if player 2 plays adjacent first?" << std::endl;
  std::cout << startingGraph->childGraphs[0]->hasWinningStrategy << std::endl;

  std::cout << "Who wins if player 2 plays non-adjacent first?" << std::endl;
  std::cout << startingGraph->childGraphs[1]->hasWinningStrategy << std::endl;

  return 0;
}
