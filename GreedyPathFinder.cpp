// GreedyPathFinder.cpp
#include "GreedyPathFinder.h"
#include <queue>
#include <cmath>
#include <limits>

struct CompareF {
    bool operator()(const std::pair<int, Node>& a, const std::pair<int, Node>& b) {
        return a.first > b.first;
    }
};

std::pair<std::vector<Node>, std::set<Node>> GreedyPathFinder::findPath(
    const std::vector<std::vector<bool>>& walls,
    const Node& start,
    const Node& end
) {
    std::map<Node, Node> cameFrom;
    std::set<Node> explored;
    std::set<Node> closed;

    std::priority_queue<
        std::pair<int, Node>,
        std::vector<std::pair<int, Node>>,
        CompareF
    > openSet;

    // Start with the initial node
    openSet.push(std::make_pair(calculateHeuristic(start, end), start));
    explored.insert(start);

    while (!openSet.empty()) {
        Node current = openSet.top().second;
        openSet.pop();

        if (closed.count(current)) continue;
        closed.insert(current);

        if (current == end) {
            return { reconstructPath(cameFrom, end), explored };
        }

        for (const auto& [neighbor, _] : getNeighbors(current, walls)) {
            if (closed.count(neighbor)) continue;

            explored.insert(neighbor);
            cameFrom[neighbor] = current;
            openSet.push(std::make_pair(calculateHeuristic(neighbor, end), neighbor));
        }
    }

    return { std::vector<Node>(), explored };
}

int GreedyPathFinder::calculateHeuristic(const Node& a, const Node& b) {
    // Octile distance heuristic - accounts for diagonal movement
    int dx = std::abs(a.x - b.x);
    int dy = std::abs(a.y - b.y);
    return 10 * (dx + dy) + (14 - 2 * 10) * std::min(dx, dy);
}

std::vector<std::pair<Node, int>> GreedyPathFinder::getNeighbors(
    const Node& node,
    const std::vector<std::vector<bool>>& walls
) {
    std::vector<std::pair<Node, int>> neighbors;
    // Cardinal and diagonal directions
    const int dx[] = { 1, -1, 0, 0, 1, 1, -1, -1 };
    const int dy[] = { 0, 0, 1, -1, 1, -1, 1, -1 };
    const int costs[] = { 10, 10, 10, 10, 14, 14, 14, 14 };

    for (int i = 0; i < 8; i++) {
        int newX = node.x + dx[i];
        int newY = node.y + dy[i];

        if (newX >= 0 && newX < Constants::GRID_WIDTH &&
            newY >= 0 && newY < Constants::GRID_HEIGHT &&
            !walls[newY][newX]) {

            // For diagonal movements, check if both cardinal neighbors are walkable
            if (i >= 4) { // Diagonal movement
                bool canMoveDiagonally = !walls[node.y][newX] && !walls[newY][node.x];
                if (!canMoveDiagonally) {
                    continue;
                }
            }

            neighbors.push_back({ Node{ newX, newY }, costs[i] });
        }
    }

    return neighbors;
}

std::vector<Node> GreedyPathFinder::reconstructPath(
    const std::map<Node, Node>& cameFrom,
    Node current
) {
    std::vector<Node> path;
    while (cameFrom.find(current) != cameFrom.end()) {
        path.push_back(current);
        current = cameFrom.at(current);
    }
    path.push_back(current);
    std::reverse(path.begin(), path.end());
    return path;
}