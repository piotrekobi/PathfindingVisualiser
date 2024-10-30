// World.h
#pragma once
#include "framework.h"
#include "Constants.h"
#include "DijkstrasPathFinder.h"
#include "AStarPathFinder.h"
#include "JPSPathFinder.h"
#include <vector>
#include <optional>
#include <set>

class World {
public:
    World();
    void setWall(int x, int y, bool state);
    bool isWall(int x, int y) const;
    void setStartPoint(int x, int y);
    void setEndPoint(int x, int y);
    void findPathDijkstra();
    void findPathAStar();
    void findPathJPS();
    void render(HDC hdc) const;

private:
    std::vector<std::vector<bool>> walls;
    std::optional<std::pair<int, int>> startPoint;
    std::optional<std::pair<int, int>> endPoint;
    std::vector<Node> currentPath;
    std::set<Node> exploredNodes;  // New: track explored nodes
    bool isValidCell(int x, int y) const;
};