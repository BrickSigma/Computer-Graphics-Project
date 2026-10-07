# Mini Game Arcade - Computer Graphics Project
This is repository contains the code for the Computer Graphics project for university. The project is a collection of simple arcade games with a style similar to that of the 1980s to the early 2000s.

> [!NOTE]  
> If you are a group member of this project, kindly check the [Group-Notes.md](Group-Notes.md) file which has details about what to do for the assignment.

## Table of contents
- [Group members and responsibilities](#group-members-and-responsibilities)
- [Technologies/tools used](#technologiestools-used)
- [Graphics techniques/algorithms implemented](#graphics-techniquesalgorithms-implemented)
- [Building and running](#building-and-running)
- [Progress Update](#progress-update)
- [Limitations and issues](#limitations-and-issues)

## Group members and responsibilities
All of the group members will be actively working on their own mini game, allowing everyone a fair chance to try each aspect of game development.

Our group members are:
- Mohamed Junaid Chaudhry - 166335
- Clyde Mugambi - 166330
- Tim Hubert Osmond - 166070
- Martha Muinde - 166319
- Hope Murimi - 150468
- Lee Gitonga - 150465

## Technologies/tools used
This project uses the following tools and libraries:
- C/C++ - the core language used for the project
- [Raylib](https://raylib.com) - the graphics library used for the project. See notes below.
- CMake - the build system used for compiling the project and managing

Raylib was chosen as the library since it is built on top of GLFW and OpenGL, providing a much simpler interface for creating application with a smaller overhead. It supports using OpenGL shaders, loading meshes and 3D object files, and handling font rendering, along with basic collision detection and math utilities which make development a lot easier.

CMake is the build system chosen for the project as it makes compiling it easier on Windows, MacOS, Linux, as well as the web, much easier.

## Graphics techniques/algorithms implemented
The following algorithms are yet to be implemented:
- [ ] Line drawing and line operations  
- [ ] Polygon representation and operation  
- [ ] Polygon clipping and filling  
- [ ] Circle and ellipse representation  
- [ ] Curves, including Bezier curves and splines  

## Building and running
In order to build the project, you only need to have a C/C++ compiler, such as MSVC or GCC, installed on your system, as well as a copy of CMake, which you can download from here: [CMake Download](https://cmake.org/download/).

> [!NOTE]  
> You do not need to download Raylib as the CMake build system has been configured to automatically download and build it for you, which should hopefully make it easier to setup and run.

From there, you can simply run the following in your favorite terminal emulator:

```bash
git clone https://github.com/BrickSigma/Computer-Graphics-Project.git
cd Computer-Graphics-Project
cmake -B build  # This generates the build files
cmake --build build  # This actually builds the project
```

Depending on what compiler you have installed, the final executable will either be in:
1. `build/Game.exe` - if you are using a Unix/Linux based system
2. `build/Debug/Game.exe` - if you are using MSVC

## Progress Update
At the time of writting this, we have only setup the basic project structure and created the template functions for the algorithms we'll need to use. Over the course of the week these will slowly be logged here as we work on them.

## Limitations and issues
As of writting this, no known limitations or issues have been faced.