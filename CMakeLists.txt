cmake_minimum_required(VERSION 3.10)
project(nidra)

set(CMAKE_CXX_STANDARD 14)

add_executable(nidra main.cpp
        code/animatedcharacter.hpp
        code/animatedcharacter.cpp
        code/animatedcharacterbuffers.cpp
        code/animatedcharacterbuffers.hpp
        code/animatedcharacterdata.cpp
        code/animatedcharacterdata.hpp
        code/animatedcharacterinputhandler.cpp
        code/animatedcharacterinputhandler.hpp
        code/texture.hpp
        code/texture.cpp
        code/shader.cpp
        code/shader.hpp code/controls.cpp code/controls.hpp)

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
    message(${GLEW_LIBRARIES})
endif ()

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(nidra ${GLFW_LIBRARIES})

find_package(glm REQUIRED)
target_link_libraries(nidra glm)

find_library(ASSIMP_LIB assimp)
if (NOT ASSIMP_LIB)
    message(WARNING "Assimp library not found explicitly. Build may still work if the linker can find it.")
    target_link_libraries(nidra assimp)
else ()
    target_link_libraries(nidra ${ASSIMP_LIB})
endif ()

target_link_libraries(nidra dl)
target_link_libraries(nidra pthread)
target_link_libraries(nidra Xrandr)
target_link_libraries(nidra Xi)
target_link_libraries(nidra X11)
target_link_libraries(nidra Xxf86vm)
target_link_libraries(nidra Xcursor)
target_link_libraries(nidra Xinerama)
target_link_libraries(nidra GLEW)
target_link_libraries(nidra GL)