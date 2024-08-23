# Platformer

This project focuses on developing a robust game and physics engine ecosystem, with a platformer game serving as a testbed for its core framework.
Built with C++ and SFML, the project emphasizes high-quality design and performance, showcasing the engine's capabilities.

## Features

- Object Management System 
  - Efficiently handles physics updates with both fixed and variable timesteps.
  - Integrates advanced collision detection mechanisms for seamless gameplay.
- Component-Based Architecture
  - Streamlines the creation and management of diverse object types.
  - Offers an intuitive interface for defining and controlling object behaviors and properties.
- Optimized Collision Detection
  - Implements the GJK (Gilbert-Johnson-Keerthi) algorithm for precise collision detection between any convex shapes.

## Build Instructions

### Prerequisites

Ensure you have the following dependencies installed before building the platformer:
1. **CMake** (version 3.24 or higher)
2. **Git** (for cloning the repository)

### Cloning the Repository and Building the Project

1. Clone the Repository to your local machine:
```bash
git clone https://github.com/SharkFinPro/Platformer.git
cd Platformer
```

2. Create a separate directory for the build process:
```bash
mkdir build
cd build
```

3. Configure the project with CMake:
```bash
cmake ..
```

4. Compile the project using your preferred build system:
```bash
cmake --build .
```

5. Run the executable located in the `bin` directory:
```bash
cd bin
./Platformer
```
