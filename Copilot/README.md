# Gobblet Gobblers

A modern C++ implementation of the Gobblet Gobblers board game.

## Overview

Gobblet Gobblers is a strategic board game similar to tic-tac-toe but with pieces of different sizes that can be stacked on top of each other.

## Project Structure

- `src/`: Source code files
- `include/`: Header files
- `assets/`: Game resources (images, sounds, etc.)
- `build/`: Build outputs
- `CMakeLists.txt`: CMake build configuration

## Building the Project

### Prerequisites

- CMake 3.14 or higher
- Modern C++ compiler (supporting C++17)
- Graphics library (SFML/SDL)

### Build Instructions

```bash
mkdir -p build && cd build
cmake ..
make
```

## Game Rules

Gobblet Gobblers is played on a 3x3 grid. Each player has three different sized pieces. A player wins by placing three of their pieces in a row, column, or diagonal. The twist is that larger pieces can cover smaller pieces.

## License

[MIT License](LICENSE)