// JPSPathFinder.h
#pragma once
#include "framework.h"
#include "Constants.h"
#include "DijkstrasPathFinder.h"
#include <vector>
#include <map>
#include <set>
#include <optional>

struct Direction {
    int dx, dy;
    bool isDiagonal() const { return dx != 0 && dy != 0; }
    Direction normalize() const {
        return Direction{
            dx != 0 ? dx / std::abs(dx) : 0,
            dy != 0 ? dy / std::abs(dy) : 0
        };
    }
};

struct JumpPoint {
    Node node;
    Direction dir;
};

class JPSPathFinder {
public:
    static std::pair<std::vector<Node>, std::set<Node>> findPath(
        const std::vector<std::vector<bool>>& walls,
        const Node& start,
        const Node& end
    );

private:
    struct NodeScore {
        Node node;
        int g;
        int f;
        Node parent;
        int dx, dy;
    };

    static bool isWalkable(const std::vector<std::vector<bool>>& walls, int x, int y);
    static bool hasForced(const std::vector<std::vector<bool>>& walls, int x, int y, const Direction& dir);
    static bool validDiagonalMove(const std::vector<std::vector<bool>>& walls, int x, int y, const Direction& dir);
    static std::optional<Node> jump(const std::vector<std::vector<bool>>& walls, int x, int y,
        const Direction& dir, const Node& end);
    static std::vector<JumpPoint> identifySuccessors(const std::vector<std::vector<bool>>& walls,
        const Node& node, const Node& end);
    static int movementCost(const Node& a, const Node& b);
    static int heuristic(const Node& a, const Node& b);
    static std::vector<Node> reconstructPath(const std::map<Node, NodeScore>& nodeInfo,
        const std::vector<std::vector<bool>>& walls, Node current);
};