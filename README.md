# Shortest Path Finder for Robot Navigation

## Project Overview

This project implements an algorithm to find the shortest path for a robot in an N*N grid, where the robot needs to move from a starting position to a target position while avoiding obstacles. The grid is represented by a matrix, and the valid moves for the robot are: Up (U), Down (D), Left (L), and Right (R).

## Problem Description

The problem is given as a file with the following format:

1. **N**: The dimensions of the grid (NxN).
2. **zx zy**: The starting coordinates of the robot (zx, zy).
3. **tx ty**: The target coordinates (tx, ty).
4. **N*N Grid**: A grid of 1's and 0's, where 1 represents an obstacle and 0 represents a free space.

Your task is to find the shortest path from the robot’s starting position (zx, zy) to the target position (tx, ty) using valid moves while avoiding obstacles.

### Example:
- N = 5 (Grid size: 5x5)
- Starting position: (0, 0)
- Target position: (4, 4)
- Grid:
    ```
    0 0 1 0 0
    0 1 0 1 0
    0 0 0 0 0
    1 1 0 1 0
    0 0 0 0 0
    ```

## Valid Moves

The robot can move in the following directions:
- **Up (U)**: Move to the cell directly above.
- **Down (D)**: Move to the cell directly below.
- **Left (L)**: Move to the cell directly left.
- **Right (R)**: Move to the cell directly right.

The robot must avoid cells marked as obstacles (1) and only move through free cells (0).

## Approach

We use a **Breadth-First Search (BFS)** algorithm to find the shortest path. The BFS ensures that we explore all possible paths level by level, guaranteeing that we find the shortest path first.


### Output:
The algorithm returns the shortest path from the robot’s starting position to the target position in the form of a sequence of moves (U, D, L, R). If no path exists, the algorithm returns "No path found".
