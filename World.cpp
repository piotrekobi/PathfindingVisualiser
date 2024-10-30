// World.cpp
#include "World.h"
#include "PathfindingStats.h"
#include <iostream>

World::World() : walls(Constants::GRID_HEIGHT, std::vector<bool>(Constants::GRID_WIDTH, false)) {
#ifdef _DEBUG
    std::cout << "World created with size: " << Constants::GRID_WIDTH << "x" << Constants::GRID_HEIGHT << std::endl;
#endif
}

bool World::isValidCell(int x, int y) const {
    return x >= 0 && x < Constants::GRID_WIDTH && y >= 0 && y < Constants::GRID_HEIGHT;
}

void World::setWall(int x, int y, bool state) {
    if (isValidCell(x, y)) {
        walls[y][x] = state;
#ifdef _DEBUG
        std::cout << "Wall at " << x << "," << y << " is now " << (walls[y][x] ? "true" : "false") << std::endl;
#endif
    }
}

bool World::isWall(int x, int y) const {
    if (isValidCell(x, y)) {
        return walls[y][x];
    }
    return false;
}

void World::setStartPoint(int x, int y) {
    if (isValidCell(x, y)) {
        startPoint = std::make_pair(x, y);
        currentPath.clear(); // Clear path when start point changes
        exploredNodes.clear(); // Clear explored nodes
#ifdef _DEBUG
        std::cout << "Start point set to: " << x << "," << y << std::endl;
#endif
    }
}

void World::setEndPoint(int x, int y) {
    if (isValidCell(x, y)) {
        endPoint = std::make_pair(x, y);
        currentPath.clear(); // Clear path when end point changes
        exploredNodes.clear(); // Clear explored nodes
#ifdef _DEBUG
        std::cout << "End point set to: " << x << "," << y << std::endl;
#endif
    }
}

void World::findPathDijkstra() {
    if (!startPoint || !endPoint) {
        return;
    }

    Node start{ startPoint->first, startPoint->second };
    Node end{ endPoint->first, endPoint->second };

    auto [path, explored] = PathfindingStats::timePathfinding(
        "Dijkstra",
        walls,
        start,
        end,
        DijkstrasPathFinder::findPath
    );

    currentPath = path;
    exploredNodes = explored;

#ifdef _DEBUG
    std::cout << "Dijkstra's path found with " << currentPath.size() << " nodes, explored "
        << exploredNodes.size() << " nodes" << std::endl;
#endif
}

void World::findPathAStar() {
    if (!startPoint || !endPoint) {
        return;
    }

    Node start{ startPoint->first, startPoint->second };
    Node end{ endPoint->first, endPoint->second };

    auto [path, explored] = PathfindingStats::timePathfinding(
        "A*",
        walls,
        start,
        end,
        AStarPathFinder::findPath
    );

    currentPath = path;
    exploredNodes = explored;

#ifdef _DEBUG
    std::cout << "A* path found with " << currentPath.size() << " nodes, explored "
        << exploredNodes.size() << " nodes" << std::endl;
#endif
}

void World::findPathJPS() {
    if (!startPoint || !endPoint) {
        return;
    }

    Node start{ startPoint->first, startPoint->second };
    Node end{ endPoint->first, endPoint->second };

    auto [path, explored] = PathfindingStats::timePathfinding(
        "JPS",
        walls,
        start,
        end,
        JPSPathFinder::findPath
    );

    currentPath = path;
    exploredNodes = explored;

#ifdef _DEBUG
    std::cout << "JPS path found with " << currentPath.size() << " nodes, explored "
        << exploredNodes.size() << " nodes" << std::endl;
#endif
}

void World::findPathGreedy() {
    if (!startPoint || !endPoint) {
        return;
    }

    Node start{ startPoint->first, startPoint->second };
    Node end{ endPoint->first, endPoint->second };

    auto [path, explored] = PathfindingStats::timePathfinding(
        "Greedy",
        walls,
        start,
        end,
        GreedyPathFinder::findPath
    );

    currentPath = path;
    exploredNodes = explored;

#ifdef _DEBUG
    std::cout << "Greedy path found with " << currentPath.size() << " nodes, explored "
        << exploredNodes.size() << " nodes" << std::endl;
#endif
}

void World::render(HDC hdc) const {
    // Fill background white
    RECT clientRect;
    GetClientRect(WindowFromDC(hdc), &clientRect);
    FillRect(hdc, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    // Create brushes
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH greenBrush = CreateSolidBrush(RGB(0, 255, 0));
    HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
    HBRUSH lightYellowBrush = CreateSolidBrush(RGB(255, 255, 200));

    // Draw explored nodes
    for (const Node& node : exploredNodes) {
        if (!(startPoint && node.x == startPoint->first && node.y == startPoint->second) &&
            !(endPoint && node.x == endPoint->first && node.y == endPoint->second)) {
            RECT exploredRect = {
                node.x * Constants::CELL_SIZE,
                node.y * Constants::CELL_SIZE,
                (node.x + 1) * Constants::CELL_SIZE,
                (node.y + 1) * Constants::CELL_SIZE
            };
            FillRect(hdc, &exploredRect, lightYellowBrush);
        }
    }

    // Draw walls
    for (int y = 0; y < Constants::GRID_HEIGHT; y++) {
        for (int x = 0; x < Constants::GRID_WIDTH; x++) {
            if (walls[y][x]) {
                RECT cellRect = {
                    x * Constants::CELL_SIZE,
                    y * Constants::CELL_SIZE,
                    (x + 1) * Constants::CELL_SIZE,
                    (y + 1) * Constants::CELL_SIZE
                };
                FillRect(hdc, &cellRect, blackBrush);
            }
        }
    }

    // Draw path if it exists
    for (const Node& node : currentPath) {
        if (!(startPoint && node.x == startPoint->first && node.y == startPoint->second) &&
            !(endPoint && node.x == endPoint->first && node.y == endPoint->second)) {
            RECT pathRect = {
                node.x * Constants::CELL_SIZE,
                node.y * Constants::CELL_SIZE,
                (node.x + 1) * Constants::CELL_SIZE,
                (node.y + 1) * Constants::CELL_SIZE
            };
            FillRect(hdc, &pathRect, blueBrush);
        }
    }

    // Draw start point if exists
    if (startPoint) {
        RECT startRect = {
            startPoint->first * Constants::CELL_SIZE,
            startPoint->second * Constants::CELL_SIZE,
            (startPoint->first + 1) * Constants::CELL_SIZE,
            (startPoint->second + 1) * Constants::CELL_SIZE
        };
        FillRect(hdc, &startRect, greenBrush);
    }

    // Draw end point if exists
    if (endPoint) {
        RECT endRect = {
            endPoint->first * Constants::CELL_SIZE,
            endPoint->second * Constants::CELL_SIZE,
            (endPoint->first + 1) * Constants::CELL_SIZE,
            (endPoint->second + 1) * Constants::CELL_SIZE
        };
        FillRect(hdc, &endRect, redBrush);
    }

    // Draw grid lines
    HPEN gridPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
    HPEN oldPen = (HPEN)SelectObject(hdc, gridPen);

    for (int x = 0; x <= Constants::GRID_WIDTH; x++) {
        MoveToEx(hdc, x * Constants::CELL_SIZE, 0, nullptr);
        LineTo(hdc, x * Constants::CELL_SIZE, Constants::GRID_HEIGHT * Constants::CELL_SIZE);
    }

    for (int y = 0; y <= Constants::GRID_HEIGHT; y++) {
        MoveToEx(hdc, 0, y * Constants::CELL_SIZE, nullptr);
        LineTo(hdc, Constants::GRID_WIDTH * Constants::CELL_SIZE, y * Constants::CELL_SIZE);
    }

    // Cleanup
    SelectObject(hdc, oldPen);
    DeleteObject(gridPen);
    DeleteObject(blackBrush);
    DeleteObject(greenBrush);
    DeleteObject(redBrush);
    DeleteObject(blueBrush);
    DeleteObject(lightYellowBrush);
}