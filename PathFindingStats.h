// PathfindingStats.h
#pragma once
#include <string>
#include <fstream>
#include <chrono>
#include <vector>
#include <functional>
#include "DijkstrasPathFinder.h"

struct PathfindingResult {
    std::string algorithm;
    size_t pathLength;
    size_t nodesVisited;
    double timeMs;
    std::string mapId;
};

class PathfindingStats {
public:
    static std::string generateMapId(const std::vector<std::vector<bool>>& walls);
    static void saveStats(const PathfindingResult& result);
    static std::pair<std::vector<Node>, std::set<Node>> timePathfinding(
        const std::string& algorithm,
        const std::vector<std::vector<bool>>& walls,
        const Node& start,
        const Node& end,
        std::function<std::pair<std::vector<Node>, std::set<Node>>(
            const std::vector<std::vector<bool>>&,
            const Node&,
            const Node&)> pathfinder
    );

private:
    static const std::string STATS_FILE;
    static std::hash<std::string> hasher;
};