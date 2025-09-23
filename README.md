# OpenGL Triangle Project

A simple OpenGL project using SDL3, gl3w, and cglm in C that renders a colorful triangle.

## Features

- **SDL3**: Window management and input handling
- **gl3w**: Modern OpenGL function loader
- **cglm**: OpenGL Mathematics library for C
- **Modern OpenGL**: Uses OpenGL 3.3 Core Profile with shaders

## Project Structure

```
oglgl/
├── .envrc              # direnv configuration
├── shell.nix           # Nix development environment
├── CMakeLists.txt      # CMake build configuration
├── Makefile           # Simple build wrapper
├── README.md          # This file
├── src/
│   └── main.c         # Main application source
└── libs/
    └── gl3w/          # gl3w OpenGL loader (generated)
```

## Building and Running

### Using Nix environment (recommended)
```bash
# Allow direnv (first time only)
direnv allow

# Build and run
make run
```

### Manual build
```bash
# Build
make build

# Run
./build/bin/oglgl
```

### Other commands
```bash
make clean    # Clean build files
make debug    # Build debug version
make help     # Show all available commands
```

## Controls

- **ESC**: Exit the application
- **Window resize**: Automatically adjusts viewport

## What it does

The program creates an 800x600 window and renders a triangle with:
- Red vertex at the top
- Green vertex at bottom left  
- Blue vertex at bottom right

The triangle uses modern OpenGL with vertex and fragment shaders, demonstrating basic OpenGL setup with SDL3.
# oglgl
# oglgl
