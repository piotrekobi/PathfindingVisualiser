// PathfindingStats.cpp
#include "PathfindingStats.h"
#include <sstream>
#include <iomanip>
#include <iostream>

const std::string PathfindingStats::STATS_FILE = "pathfinding_stats.csv";
std::hash<std::string> PathfindingStats::hasher;

std::string PathfindingStats::generateMapId(const std::vector<std::vector<bool>>& walls) {
    std::stringstream ss;

    // Add dimensions
    ss << walls.size() << "x" << walls[0].size() << "_";

    // Create a string representation of the wall layout
    for (const auto& row : walls) {
        for (bool wall : row) {
            ss << (wall ? '1' : '0');
        }
    }

    // Generate hash of the string
    size_t hash = hasher(ss.str());

    // Convert hash to hex string
    std::stringstream hashStream;
    hashStream << std::hex << std::setfill('0') << std::setw(16) << hash;

    return hashStream.str();
}

void PathfindingStats::saveStats(const PathfindingResult& result) {
    bool fileExists = std::ifstream(STATS_FILE).good();
    std::ofstream file(STATS_FILE, std::ios::app);

    // Write header if file is new
    if (!fileExists) {
        file << "MapID,Algorithm,PathLength,NodesVisited,TimeMs\n";
    }

    // Write stats
    file << result.mapId << ","
        << result.algorithm << ","
        << result.pathLength << ","
        << result.nodesVisited << ","
        << std::fixed << std::setprecision(3) << result.timeMs << "\n";
}

std::pair<std::vector<Node>, std::set<Node>> PathfindingStats::timePathfinding(
    const std::string& algorithm,
    const std::vector<std::vector<bool>>& walls,
    const Node& start,
    const Node& end,
    std::function<std::pair<std::vector<Node>, std::set<Node>>(
        const std::vector<std::vector<bool>>&,
        const Node&,
        const Node&)> pathfinder
) {
    // Start timing
    auto startTime = std::chrono::high_resolution_clock::now();

    // Run pathfinding
    auto result = pathfinder(walls, start, end);

    // End timing
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    // Create and save stats
    PathfindingResult stats{
        algorithm,
        result.first.size(),
        result.second.size(),
        duration.count() / 1000.0, // Convert to milliseconds
        generateMapId(walls)
    };

    saveStats(stats);

    return result;
}