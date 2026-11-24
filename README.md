# Bus Routes — Data Structures

A C++ project implementing a data structure for managing a public transport network. The project focuses on efficient data storage, graph traversal, and route-finding algorithms.

## Overview

The implementation models a public transport network consisting of stops, regions, and routes. It supports operations for managing these entities and finding journeys between stops using different graph algorithms.

## Features

- **Stop management:** Add, remove, search, and retrieve information about stops.
- **Region management:** Create regions, organize hierarchical relationships between regions, and associate stops with regions.
- **Route management:** Add, remove, and inspect routes connecting multiple stops.
- **Journey planning:** Find journeys between stops using different optimization criteria.

## Algorithms

The project implements several graph algorithms:

| Algorithm | Purpose |
|---|---|
| Breadth-First Search (BFS) | Find a journey between two stops |
| BFS | Find a journey with the fewest stops |
| Depth-First Search (DFS) | Find a cycle in the network |
| Dijkstra's Algorithm | Find the shortest journey by distance |

## Data Structures

The implementation uses C++ Standard Library containers, including:

- `std::unordered_map` for storing stops, regions, and routes.
- `std::unordered_set` for tracking visited nodes.
- `std::vector` for storing route stops and region relationships.
- `std::queue` for breadth-first traversal.
- `std::priority_queue` for Dijkstra's algorithm.

## Performance

Performance was evaluated using the provided performance testing tool, including tests with up to **1,000,000 entries**.

Lookup operations for stops and regions demonstrated approximately constant-time average performance.

## Technologies

- C++
- C++ Standard Library
- Graph Algorithms
- Data Structures and Algorithm Analysis

## Project Context

Developed as part of a university Data Structures and Algorithms course. The project emphasizes implementing efficient data structures and graph algorithms while considering correctness and computational complexity.
