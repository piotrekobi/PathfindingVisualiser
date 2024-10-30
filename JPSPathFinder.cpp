// JPSPathFinder.cpp
#include "JPSPathFinder.h"
#include <queue>
#include <cmath>
#include <algorithm>

bool JPSPathFinder::isWalkable(const std::vector<std::vector<bool>>& walls, int x, int y) {
    return x >= 0 && x < Constants::GRID_WIDTH && y >= 0 && y < Constants::GRID_HEIGHT && !walls[y][x];
}

bool JPSPathFinder::validDiagonalMove(const std::vector<std::vector<bool>>& walls, int x, int y, const Direction& dir) {
    return isWalkable(walls, x + dir.dx, y + dir.dy) &&
        isWalkable(walls, x + dir.dx, y) &&
        isWalkable(walls, x, y + dir.dy);
}

bool JPSPathFinder::hasForced(const std::vector<std::vector<bool>>& walls, int x, int y, const Direction& dir) {
    if (dir.isDiagonal()) {
        return (!isWalkable(walls, x + dir.dx, y) && isWalkable(walls, x + dir.dx, y + dir.dy)) ||
            (!isWalkable(walls, x, y + dir.dy) && isWalkable(walls, x + dir.dx, y + dir.dy));
    }
    if (dir.dx != 0) {
        return (!isWalkable(walls, x, y + 1) && isWalkable(walls, x + dir.dx, y + 1)) ||
            (!isWalkable(walls, x, y - 1) && isWalkable(walls, x + dir.dx, y - 1));
    }
    if (dir.dy != 0) {
        return (!isWalkable(walls, x + 1, y) && isWalkable(walls, x + 1, y + dir.dy)) ||
            (!isWalkable(walls, x - 1, y) && isWalkable(walls, x - 1, y + dir.dy));
    }
    return false;
}

std::optional<Node> JPSPathFinder::jump(const std::vector<std::vector<bool>>& walls,
    int x, int y, const Direction& dir, const Node& end) {
    int nextX = x + dir.dx;
    int nextY = y + dir.dy;

    if (!isWalkable(walls, nextX, nextY)) return std::nullopt;
    if (dir.isDiagonal() && !validDiagonalMove(walls, x, y, dir)) return std::nullopt;
    if (Node{ nextX, nextY } == end) return Node{ nextX, nextY };
    if (hasForced(walls, nextX, nextY, dir)) return Node{ nextX, nextY };

    if (dir.isDiagonal()) {
        if (jump(walls, nextX, nextY, Direction{ dir.dx, 0 }, end) ||
            jump(walls, nextX, nextY, Direction{ 0, dir.dy }, end)) {
            return Node{ nextX, nextY };
        }
    }

    return jump(walls, nextX, nextY, dir, end);
}

std::vector<JumpPoint> JPSPathFinder::identifySuccessors(const std::vector<std::vector<bool>>& walls,
    const Node& node, const Node& end) {
    std::vector<JumpPoint> successors;
    const Direction directions[] = {
        {1,0}, {-1,0}, {0,1}, {0,-1}, {1,1}, {-1,1}, {1,-1}, {-1,-1}
    };

    for (const auto& dir : directions) {
        if (dir.isDiagonal() && !validDiagonalMove(walls, node.x, node.y, dir)) continue;
        if (!dir.isDiagonal() && !isWalkable(walls, node.x + dir.dx, node.y + dir.dy)) continue;

        if (auto jp = jump(walls, node.x, node.y, dir, end)) {
            successors.push_back({ *jp, dir });
        }
    }
    return successors;
}

int JPSPathFinder::movementCost(const Node& a, const Node& b) {
    return (a.x != b.x && a.y != b.y) ? 14 : 10;
}

int JPSPathFinder::heuristic(const Node& a, const Node& b) {
    int dx = std::abs(a.x - b.x);
    int dy = std::abs(a.y - b.y);
    return 10 * (dx + dy) + (14 - 2 * 10) * std::min(dx, dy);
}

std::vector<Node> JPSPathFinder::reconstructPath(const std::map<Node, NodeScore>& nodeInfo,
    const std::vector<std::vector<bool>>& walls, Node current) {
    std::vector<Node> path;
    while (nodeInfo.find(current) != nodeInfo.end()) {
        const auto& info = nodeInfo.at(current);
        if (current == info.parent) break;

        Node from = info.parent;
        int dx = (current.x - from.x) != 0 ? (current.x - from.x) / std::abs(current.x - from.x) : 0;
        int dy = (current.y - from.y) != 0 ? (current.y - from.y) / std::abs(current.y - from.y) : 0;

        for (Node step = current; step.x != from.x || step.y != from.y; step.x -= dx, step.y -= dy) {
            path.push_back(step);
        }
        current = from;
    }
    path.push_back(current);
    std::reverse(path.begin(), path.end());
    return path;
}

std::pair<std::vector<Node>, std::set<Node>> JPSPathFinder::findPath(
    const std::vector<std::vector<bool>>& walls,
    const Node& start,
    const Node& end) {
    auto compare = [](const std::pair<int, NodeScore>& a, const std::pair<int, NodeScore>& b) {
        return a.first > b.first;
        };

    std::priority_queue<std::pair<int, NodeScore>, std::vector<std::pair<int, NodeScore>>, decltype(compare)> openSet(compare);
    std::map<Node, NodeScore> nodeInfo;
    std::set<Node> closedSet;
    std::set<Node> explored;

    NodeScore startScore{ start, 0, heuristic(start, end), start, 0, 0 };
    openSet.push({ startScore.f, startScore });
    nodeInfo[start] = startScore;
    explored.insert(start);

    while (!openSet.empty()) {
        NodeScore current = openSet.top().second;
        openSet.pop();

        if (current.node == end) {
            return { reconstructPath(nodeInfo, walls, end), explored };
        }

        if (closedSet.count(current.node)) continue;
        closedSet.insert(current.node);

        for (const auto& successor : identifySuccessors(walls, current.node, end)) {
            if (closedSet.count(successor.node)) continue;

            explored.insert(successor.node);
            int newG = current.g + movementCost(current.node, successor.node);
            int newF = newG + heuristic(successor.node, end);

            auto existing = nodeInfo.find(successor.node);
            if (existing == nodeInfo.end() || newG < existing->second.g) {
                NodeScore newScore{ successor.node, newG, newF, current.node, successor.dir.dx, successor.dir.dy };
                openSet.push({ newF, newScore });
                nodeInfo[successor.node] = newScore;
            }
        }
    }

    return { std::vector<Node>(), explored };
}