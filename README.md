# Snake Game (SDL3)

A classic Snake game implementation in C++ using the **SDL3** library.

## Features

- **Classic Gameplay**: Navigate the snake to eat apples and grow longer.
- **Dynamic Growth**: The snake increases in length with each apple consumed.
- **Screen Wrapping**: The snake wraps around the edges of the window (default behavior).
- **Smooth Rendering**: High-DPI support and hardware-accelerated rendering using SDL3.
- **Game Over Detection**: The game ends if the snake collides with itself.

## Controls

- **Up Arrow**: Move Up
- **Right Arrow**: Move Right
- **Down Arrow**: Move Down
- **Left Arrow**: Move Left
- **Q Key**: Quit Game

## Prerequisites

To build and run this project, you need:

- A C++ compiler supporting **C++17** or later (e.g., GCC/MinGW-w64).
- [SDL3 Library](https://www.libsdl.org/) installed and configured in your environment.
- On Windows, `SDL3.dll` must be in the same directory as the executable.

## Building the Project

### Using MinGW/GCC (Windows)

Assuming you have `g++` and SDL3 installed, you can compile the project with:

```bash
g++ main.cpp -o snake.exe -lSDL3
```

Ensure the SDL3 include and library paths are correctly set if they are not in your system's default search paths.

### Running

Simply execute the compiled binary:

```bash
./snake.exe
```

## Project Structure

- `main.cpp`: The core game logic and SDL3 initialization.
- `SDL3.dll`: The SDL3 dynamic link library (Windows).
- `.vscode/`: VS Code configuration for IntelliSense and building.

## License

This project is for educational purposes. Feel free to use and modify it!
