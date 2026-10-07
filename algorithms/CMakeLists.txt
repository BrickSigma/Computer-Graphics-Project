add_library(Algorithms)

target_sources(Algorithms
    PRIVATE
        src/line.cpp
        src/polygon.cpp
        src/circle.cpp
        src/curves.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            include
        FILES
            include/algorithms.hpp
            include/algorithms/line.hpp
            include/algorithms/polygon.hpp
            include/algorithms/circle.hpp
            include/algorithms/curves.hpp
)

target_link_libraries(Algorithms PRIVATE raylib)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    set_target_properties(Algorithms PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(Algorithms "-framework IOKit")
    target_link_libraries(Algorithms "-framework Cocoa")
    target_link_libraries(Algorithms "-framework OpenGL")
    target_link_libraries(Algorithms "-framework QuartzCore")
endif()