// DijkstrasPathFinder.h
#pragma once
#include "framework.h"
#include "Constants.h"
#include <vector>
#include <queue>
#include <map>
#include <set>

struct Node {
    int x, y;
    bool operator==(const Node& other) const {
        return x == other.x && y == other.y;
    }
    bool operator<(const Node& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};

class DijkstrasPathFinder {
public:
    static std::pair<std::vector<Node>, std::set<Node>> findPath(
        const std::vector<std::vector<bool>>& walls,
        const Node& start,
        const Node& end
    );

private:
    static std::vector<std::pair<Node, int>> getNeighbors(
        const Node& node,
        const std::vector<std::vector<bool>>& walls
    );
    static std::vector<Node> reconstructPath(
        const std::map<Node, Node>& cameFrom,
        Node current
    );
};