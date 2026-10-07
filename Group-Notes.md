# Note to group members
This is meant for our group's members to read and understand who will be doing what.

The first assignment for the project is to implement the following algorithms in C/C++:
- Line drawing and line operations
- Polygon representation and operation
- Polygon clipping and filling
- Circle and ellipse representation
- Curves, including Bezier curves and splines

The code for the algorithms is in the [./algorithms](algorithms/) folder. The folder is broken into two subfolders:
1. [src/ folder](algorithms/src/) - contains the C++ implementation for the each algorithm, which have their dedicated names,
2. [include/ folder](algorithms/include/) - contains the C++ header files for the function declarations.

It's important to note that all functions, structs, and variables in the algorithms folder must be placed under the namespace `Algorithms`. This is to prevent any conflict with Raylib's built in functions. You can look at the [main.cpp](src/main.cpp) file to see how they are used.
