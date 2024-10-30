// AStarPathFinder.cpp
#include "AStarPathFinder.h"
#include <queue>
#include <map>
#include <cmath>
#include <limits>

struct CompareF {
    bool operator()(const std::pair<int, Node>& a, const std::pair<int, Node>& b) {
        return a.first > b.first;
    }
};

std::pair<std::vector<Node>, std::set<Node>> AStarPathFinder::findPath(
    const std::vector<std::vector<bool>>& walls,
    const Node& start,
    const Node& end
) {
    std::map<Node, int> gScore;
    std::map<Node, int> fScore;
    std::map<Node, Node> cameFrom;
    std::set<Node> explored;

    std::priority_queue<
        std::pair<int, Node>,
        std::vector<std::pair<int, Node>>,
        CompareF
    > openSet;

    // Initialize scores
    for (int y = 0; y < Constants::GRID_HEIGHT; y++) {
        for (int x = 0; x < Constants::GRID_WIDTH; x++) {
            Node node{ x, y };
            gScore[node] = (std::numeric_limits<int>::max)();
            fScore[node] = (std::numeric_limits<int>::max)();
        }
    }

    gScore[start] = 0;
    fScore[start] = calculateHeuristic(start, end);
    openSet.push(std::make_pair(fScore[start], start));

    while (!openSet.empty()) {
        Node current = openSet.top().second;
        openSet.pop();

        explored.insert(current);  // Mark node as explored

        if (current == end) {
            return { reconstructPath(cameFrom, end), explored };
        }

        for (const auto& [neighbor, cost] : getNeighbors(current, walls)) {
            int tentativeGScore = gScore[current] + cost;

            if (tentativeGScore < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentativeGScore;
                fScore[neighbor] = gScore[neighbor] + calculateHeuristic(neighbor, end);
                openSet.push(std::make_pair(fScore[neighbor], neighbor));
            }
        }
    }

    return { std::vector<Node>(), explored }; // Return empty path and explored nodes if no path found
}

int AStarPathFinder::calculateHeuristic(const Node& a, const Node& b) {
    // Octile distance heuristic - accounts for diagonal movement
    int dx = std::abs(a.x - b.x);
    int dy = std::abs(a.y - b.y);
    return 10 * (dx + dy) + (14 - 2 * 10) * std::min(dx, dy);
}

std::vector<std::pair<Node, int>> AStarPathFinder::getNeighbors(
    const Node& node,
    const std::vector<std::vector<bool>>& walls
) {
    std::vector<std::pair<Node, int>> neighbors;
    // Cardinal and diagonal directions
    const int dx[] = { 1, -1, 0, 0, 1, 1, -1, -1 };
    const int dy[] = { 0, 0, 1, -1, 1, -1, 1, -1 };
    const int costs[] = { 10, 10, 10, 10, 14, 14, 14, 14 }; // 10 for cardinal, 14 for diagonal (≈10*√2)

    for (int i = 0; i < 8; i++) {
        int newX = node.x + dx[i];
        int newY = node.y + dy[i];

        if (newX >= 0 && newX < Constants::GRID_WIDTH &&
            newY >= 0 && newY < Constants::GRID_HEIGHT &&
            !walls[newY][newX]) {

            // For diagonal movements, check if both cardinal neighbors are walkable
            // to prevent corner cutting
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

std::vector<Node> AStarPathFinder::reconstructPath(
    const std::map<Node, Node>& cameFrom,
    Node current
) {
    std::vector<Node> path;
    while (cameFrom.find(current) != cameFrom.end()) {
        path.push_back(current);
        current = cameFrom.at(current);
    }
    path.push_back(current);  // Add the start node
    std::reverse(path.begin(), path.end());
    return path;
}