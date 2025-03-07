/////////////////////////////////////////////////////////////////////////

// STL A* Search implementation
// (C)2001 Justin Heyes-Jones
//
// Finding a path on a simple grid maze
// This shows how to do shortest path finding using A*

//////////////////////////////////////////////////////////////////////////

#include "../include/stlastar.h"

#include <math.h>
#include <stdio.h>
#include <iostream>

constexpr int DEBUG_LISTS = 0;
constexpr int DEBUG_LIST_LENGTHS_ONLY = 0;

// Global data

// The world map

const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 20;

int world_map[MAP_WIDTH * MAP_HEIGHT] = {

    // 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 00
    1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,  // 01
    1, 9, 9, 1, 1, 9, 9, 9, 1, 9, 1, 9, 1, 9, 1, 9, 9, 9, 1, 1,  // 02
    1, 9, 9, 1, 1, 9, 9, 9, 1, 9, 1, 9, 1, 9, 1, 9, 9, 9, 1, 1,  // 03
    1, 9, 1, 1, 1, 1, 9, 9, 1, 9, 1, 9, 1, 1, 1, 1, 9, 9, 1, 1,  // 04
    1, 9, 1, 1, 9, 1, 1, 1, 1, 9, 1, 1, 1, 1, 9, 1, 1, 1, 1, 1,  // 05
    1, 9, 9, 9, 9, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 1, 1, 1, 1, 1,  // 06
    1, 9, 9, 9, 9, 9, 9, 9, 9, 1, 1, 1, 9, 9, 9, 9, 9, 9, 9, 1,  // 07
    1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1,  // 08
    1, 9, 1, 9, 9, 9, 9, 9, 9, 9, 1, 1, 9, 9, 9, 9, 9, 9, 9, 1,  // 09
    1, 9, 1, 1, 1, 1, 9, 1, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 10
    1, 9, 9, 9, 9, 9, 1, 9, 1, 9, 1, 9, 9, 9, 9, 9, 1, 1, 1, 1,  // 11
    1, 9, 1, 9, 1, 9, 9, 9, 1, 9, 1, 9, 1, 9, 1, 9, 9, 9, 1, 1,  // 12
    1, 9, 1, 9, 1, 9, 9, 9, 1, 9, 1, 9, 1, 9, 1, 9, 9, 9, 1, 1,  // 13
    1, 9, 1, 1, 1, 1, 9, 9, 1, 9, 1, 9, 1, 1, 1, 1, 9, 9, 1, 1,  // 14
    1, 9, 1, 1, 9, 1, 1, 1, 1, 9, 1, 1, 1, 1, 9, 1, 1, 1, 1, 1,  // 15
    1, 9, 9, 9, 9, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 1, 1, 1, 1, 1,  // 16
    1, 1, 9, 9, 9, 9, 9, 9, 9, 1, 1, 1, 9, 9, 9, 1, 9, 9, 9, 9,  // 17
    1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1,  // 18
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 19

};

// map helper functions

int GetMap(int x, int y) {
  if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
    return 9;
  }

  return world_map[(y * MAP_WIDTH) + x];
}

// Definitions

// To use the search class you must define the following calls...

// Data
//    Your own state space information
// Functions
//    (Optional) Constructor.
//    Nodes are created by the user, so whether you use a
//      constructor with parameters as below, or just set the object up after
//      the constructor, is up to you.
//
//    (Optional) Destructor.
//    The destructor will be called if you create one. You
//    can rely on the default constructor unless you dynamically
// allocate something in    your data
//
//    float GoalDistanceEstimate( PuzzleState &nodeGoal );
//    Return the estimated cost to goal from this node (pass reference
// to goal node)
//
//    bool IsGoal( PuzzleState &nodeGoal );
//    Return true if this node is the goal.
//
//    bool GetSuccessors( AStarSearch<PuzzleState> *astarsearch );
//    For each successor to this state call the AStarSearch's
// AddSuccessor call to     add each one to the current search -
// return false if you are out of memory and the search     will
// fail
//
//    float GetCost( PuzzleState *successor );
//    Return the cost moving from this state to the state of successor
//
//    bool IsSameState( PuzzleState &rhs );
//    Return true if the provided state is the same as this state

class MapSearchNode {
 public:
  int x;  // the (x,y) positions of the node
  int y;

  MapSearchNode() : x(0), y(0) {}
  MapSearchNode(int px, int py) : x(px), y(py) {}
  // Here's the heuristic function that estimates the distance from a Node
  // to the Goal.
  float GoalDistanceEstimate(MapSearchNode &nodeGoal) {
    return abs(x - nodeGoal.x) + abs(y - nodeGoal.y);
  }  // GoalDistanceEstimate

  bool IsGoal(MapSearchNode &nodeGoal) {
    if ((x == nodeGoal.x) && (y == nodeGoal.y)) {
      return true;
    }

    return false;
  }  // IsGoal

  // This generates the successors to the given Node. It uses a helper function
  // called AddSuccessor to give the successors to the AStar class. The A*
  // specific initialisation is done for each node internally, so here you just
  // set the state information that is specific to the application
  bool GetSuccessors(AStarSearch<MapSearchNode> *astarsearch,
                     MapSearchNode *parent_node) {
    int parent_x = -1;
    int parent_y = -1;

    if (parent_node) {
      parent_x = parent_node->x;
      parent_y = parent_node->y;
    }

    MapSearchNode NewNode;

    // push each possible move except allowing the search to go backwards

    if ((GetMap(x - 1, y) < 9) && !((parent_x == x - 1) && (parent_y == y))) {
      NewNode = MapSearchNode(x - 1, y);
      astarsearch->AddSuccessor(NewNode);
    }

    if ((GetMap(x, y - 1) < 9) && !((parent_x == x) && (parent_y == y - 1))) {
      NewNode = MapSearchNode(x, y - 1);
      astarsearch->AddSuccessor(NewNode);
    }

    if ((GetMap(x + 1, y) < 9) && !((parent_x == x + 1) && (parent_y == y))) {
      NewNode = MapSearchNode(x + 1, y);
      astarsearch->AddSuccessor(NewNode);
    }

    if ((GetMap(x, y + 1) < 9) && !((parent_x == x) && (parent_y == y + 1))) {
      NewNode = MapSearchNode(x, y + 1);
      astarsearch->AddSuccessor(NewNode);
    }

    return true;
  }  // GetSuccessors

  // given this node, what does it cost to move to successor. In the case
  // of our map the answer is the map terrain value at this node since that is
  // conceptually where we're moving
  float GetCost(MapSearchNode &successor) {
    return (float)GetMap(x, y);
  }  // GetCost

  bool IsSameState(MapSearchNode &rhs) {
    // same state in a maze search is simply when (x,y) are the same
    if ((x == rhs.x) && (y == rhs.y)) {
      return true;
    } else {
      return false;
    }
  }  // IsSameState

  void PrintNodeInfo() {
    char str[100];
    sprintf(str, "Node position : (%d,%d)\n", x, y);

    std::cout << str;
  }  // PrintNodeInfo
};

// Main
int main() {
  std::cout << "STL A* Search implementation\n";

  // Our sample problem defines the world as a 2d array representing a terrain
  // Each element contains an integer from 0 to 5 which indicates the cost
  // of travel across the terrain. Zero means the least possible difficulty
  // in travelling (think ice rink if you can skate) whilst 5 represents the
  // most difficult. 9 indicates that we cannot pass.

  // Create an instance of the search class...
  AStarSearch<MapSearchNode> astarsearch;

  unsigned int SearchCount = 0;

  const unsigned int NumSearches = 1;

  while (SearchCount < NumSearches) {
    // Create a start state
    MapSearchNode nodeStart;
    nodeStart.x = rand() % MAP_WIDTH;
    nodeStart.y = rand() % MAP_HEIGHT;

    // Define the goal state
    MapSearchNode nodeEnd;
    nodeEnd.x = rand() % MAP_WIDTH;
    nodeEnd.y = rand() % MAP_HEIGHT;

    // Set Start and goal states

    astarsearch.SetStartAndGoalStates(nodeStart, nodeEnd);

    unsigned int SearchState;
    unsigned int SearchSteps = 0;

    do {
      SearchState = astarsearch.SearchStep();

      SearchSteps++;

      if constexpr (DEBUG_LISTS == 1) {
        std::cout << "Steps:" << SearchSteps << "\n";

        int len = 0;

        std::cout << "Open:\n";
        MapSearchNode *p = astarsearch.GetOpenListStart();
        while (p) {
          len++;
          if constexpr (DEBUG_LIST_LENGTHS_ONLY == 0)
            ((MapSearchNode *)p)->PrintNodeInfo();
          p = astarsearch.GetOpenListNext();
        }

        std::cout << "Open list has " << len << " nodes\n";

        len = 0;

        std::cout << "Closed:\n";
        p = astarsearch.GetClosedListStart();
        while (p) {
          len++;
          if constexpr (DEBUG_LIST_LENGTHS_ONLY == 0) p->PrintNodeInfo();
          p = astarsearch.GetClosedListNext();
        }

        std::cout << "Closed list has " << len << " nodes\n";
      }

    } while (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING);

    if (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED) {
      std::cout << "Search found goal state\n";

      MapSearchNode *node = astarsearch.GetSolutionStart();

      std::cout << "Displaying solution\n";
      int steps = 0;

      node->PrintNodeInfo();
      for (;;) {
        node = astarsearch.GetSolutionNext();

        if (!node) {
          break;
        }

        node->PrintNodeInfo();
        steps++;
      };

      std::cout << "Solution steps " << steps << std::endl;

      // Once you're done with the solution you can free the nodes up
      astarsearch.FreeSolutionNodes();

    } else if (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED) {
      std::cout << "Search terminated. Did not find goal state\n";
    }

    // Display the number of loops the search went through
    std::cout << "SearchSteps : " << SearchSteps << "\n";

    SearchCount++;

    astarsearch.EnsureMemoryFreed();
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////////////////
