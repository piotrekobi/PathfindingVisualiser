// GreedyPathFinder.h
#pragma once
#include "framework.h"
#include "Constants.h"
#include "DijkstrasPathFinder.h"
#include <vector>
#include <map>
#include <set>

class GreedyPathFinder {
public:
    static std::pair<std::vector<Node>, std::set<Node>> findPath(
        const std::vector<std::vector<bool>>& walls,
        const Node& start,
        const Node& end
    );

private:
    static int calculateHeuristic(const Node& a, const Node& b);
    static std::vector<std::pair<Node, int>> getNeighbors(
        const Node& node,
        const std::vector<std::vector<bool>>& walls
    );
    static std::vector<Node> reconstructPath(
        const std::map<Node, Node>& cameFrom,
        Node current
    );
};