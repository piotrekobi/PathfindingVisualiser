# Pathfinding Visualizer

A Windows application that visualizes different pathfinding algorithms in real-time, including A*, Dijkstra's, Jump Point Search (JPS), and Greedy Best-First Search. The application allows users to draw walls, set start/end points, and compare the performance of different pathfinding algorithms.

## Features

- **Multiple Pathfinding Algorithms**:
  - A* (balanced performance with optimal paths)
  - Dijkstra's (guarantees optimal paths)
  - Jump Point Search (optimized for uniform cost grids)
  - Greedy Best-First Search (fast but non-optimal paths)

- **Interactive Grid**:
  - Left-click and drag to draw/erase walls
  - Right-click to set start point
  - Ctrl + Right-click to set end point
  - Grid size: 500x500 cells
  
- **Visualization**:
  - Green cell: Start point
  - Red cell: End point
  - Black cells: Walls
  - Light yellow cells: Explored nodes
  - Blue cells: Final path

- **Performance Statistics**:
  - Automatically tracks and saves performance metrics to CSV
  - Records path length, nodes visited, and execution time
  - Unique map IDs for comparing algorithm performance

## Controls

- `A`: Run A* pathfinding
- `D`: Run Dijkstra's pathfinding
- `J`: Run Jump Point Search
- `G`: Run Greedy Best-First Search
- Left Mouse Button: Draw/erase walls
- Right Mouse Button: Set start point
- Ctrl + Right Mouse Button: Set end point

## Technical Details

### Project Structure

- `World`: Manages the grid state and rendering
- `PathfindingStats`: Handles performance tracking and statistics
- Algorithm Implementations:
  - `AStarPathFinder`
  - `DijkstrasPathFinder`
  - `JPSPathFinder`
  - `GreedyPathFinder`

### Pathfinding Features

- Diagonal movement support
- Corner-cutting prevention
- Uniform cost for cardinal movements (10)
- Weighted diagonal cost (14, approximating √2 * 10)
- Octile distance heuristic for A* and JPS

### Performance Tracking

Statistics are saved to `pathfinding_stats.csv` with the following data:
- MapID: Unique identifier for the wall configuration
- Algorithm: Name of the pathfinding algorithm used
- PathLength: Number of cells in the final path
- NodesVisited: Total nodes explored during search
- TimeMs: Execution time in milliseconds

## Building the Project

### Requirements

- Windows OS
- Visual Studio 2019 or later
- Windows SDK

### Build Instructions

1. Open the solution in Visual Studio
2. Select Release or Debug configuration
3. Build the solution (F7)
4. Run the application (F5)

## Implementation Notes

- Uses Win32 API for rendering
- Debug mode includes console output for debugging
- Grid cell size is configurable in Constants.h
- All algorithms support diagonal movement
- JPS implementation includes pruning rules for speed optimization
- Performance statistics are automatically collected during runtime

## Debug Features

When built in Debug mode:
- Console window for debug output
- Detailed logging of:
  - Wall placement/removal
  - Start/end point changes
  - Pathfinding results
  - Performance metrics

## Performance Considerations

- A*: Best balance of speed and path optimality
- Dijkstra's: Slowest but guarantees optimal paths
- JPS: Fastest for uniform cost grids with few obstacles
- Greedy: Fastest but may produce suboptimal paths

Memory usage is optimized through:
- Efficient grid representation using boolean vectors
- Smart pointers for resource management
- Set data structure for tracking explored nodes

