# Platformer

This project aims to develop a robust game engine ecosystem, with a platformer serving as the test bed for its framework.
Built using C++ and SFML, the project emphasizes high-quality design and performance.

## Build Instructions

### Prerequisites

Before building the platformer, ensure you have the following dependencies installed:

1. **CMake** (version 3.24 or higher)
2. **Git** (for cloning the repository)

### Cloning the Repository and Building the Project

1. Clone the Repository

First, clone the repository to your local machine:

```bash
git clone https://github.com/SharkFinPro/Platformer.git
cd Platformer
```

2. Create a Build Directory

Create a separate directory for the build process:

```bash
mkdir build
cd build
```

3. Generate Build Files with CMake

Configure the CMake project and generate the necessary build files:

```bash
cmake ..
```

4. Build the Project

Compile the project using your preferred build system:

```bash
cmake --build .
```

5. Run the Executable

After building, all files will have been written to the `bin` directory. You can run the platformer with:

```bash
cd bin
./Platformer
```
