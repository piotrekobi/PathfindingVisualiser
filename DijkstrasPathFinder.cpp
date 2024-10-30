// DijkstrasPathFinder.cpp
#include "DijkstrasPathFinder.h"
#include <queue>
#include <map>
#include <limits>

struct CompareDistance {
    bool operator()(const std::pair<int, Node>& a, const std::pair<int, Node>& b) {
        return a.first > b.first;
    }
};

std::pair<std::vector<Node>, std::set<Node>> DijkstrasPathFinder::findPath(
    const std::vector<std::vector<bool>>& walls,
    const Node& start,
    const Node& end
) {
    std::map<Node, int> distances;
    std::map<Node, Node> cameFrom;
    std::set<Node> explored;

    std::priority_queue<
        std::pair<int, Node>,
        std::vector<std::pair<int, Node>>,
        CompareDistance
    > pq;

    // Initialize distances
    for (int y = 0; y < Constants::GRID_HEIGHT; y++) {
        for (int x = 0; x < Constants::GRID_WIDTH; x++) {
            distances[Node{ x, y }] = (std::numeric_limits<int>::max)();
        }
    }

    distances[start] = 0;
    pq.push(std::make_pair(0, start));

    while (!pq.empty()) {
        Node current = pq.top().second;
        int currentDist = pq.top().first;
        pq.pop();

        explored.insert(current);  // Mark node as explored

        if (current == end) {
            return { reconstructPath(cameFrom, end), explored };
        }

        if (currentDist > distances[current]) {
            continue;
        }

        for (const auto& [neighbor, cost] : getNeighbors(current, walls)) {
            int newDist = distances[current] + cost;

            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                cameFrom[neighbor] = current;
                pq.push(std::make_pair(newDist, neighbor));
            }
        }
    }

    return { std::vector<Node>(), explored }; // Return empty path and explored nodes if no path found
}

std::vector<std::pair<Node, int>> DijkstrasPathFinder::getNeighbors(
    const Node& node,
    const std::vector<std::vector<bool>>& walls
) {
    std::vector<std::pair<Node, int>> neighbors;
    // Cardinal directions (cost 10)
    const int dx[] = { 1, -1, 0, 0, 1, 1, -1, -1 };
    const int dy[] = { 0, 0, 1, -1, 1, -1, 1, -1 };
    const int costs[] = { 10, 10, 10, 10, 14, 14, 14, 14 }; // 10 for cardinal, 14 for diagonal (≈10*√2)

    for (int i = 0; i < 8; i++) {
        int newX = node.x + dx[i];
        int newY = node.y + dy[i];

        // Check if the new position is within bounds and not a wall
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

std::vector<Node> DijkstrasPathFinder::reconstructPath(
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