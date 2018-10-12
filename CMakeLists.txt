cmake_minimum_required(VERSION 3.10)
project(nidra)

set(CMAKE_CXX_STANDARD 14)

add_executable(driver driver/main.cpp
        code/scene/animatedcharacter.hpp
        code/scene/animatedcharacter.cpp
        code/realtime/animatedcharacterbuffers.cpp
        code/realtime/animatedcharacterbuffers.hpp
        code/scene/animatedcharacterdata.cpp
        code/scene/animatedcharacterdata.hpp
        code/scene/animatedcharacterinputhandler.cpp
        code/scene/animatedcharacterinputhandler.hpp
        code/utilities/texture.hpp
        code/utilities/texture.cpp
        code/utilities/shader.cpp
        code/utilities/shader.hpp code/utilities/controls.cpp code/utilities/controls.hpp)

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
    message(${GLEW_LIBRARIES})
endif ()

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(driver ${GLFW_LIBRARIES})

find_package(glm REQUIRED)
target_link_libraries(driver glm)

find_library(ASSIMP_LIB assimp)
if (NOT ASSIMP_LIB)
    message(WARNING "Assimp library not found explicitly. Build may still work if the linker can find it.")
    target_link_libraries(driver assimp)
else ()
    target_link_libraries(driver ${ASSIMP_LIB})
endif ()

target_link_libraries(driver dl)
target_link_libraries(driver pthread)
target_link_libraries(driver Xrandr)
target_link_libraries(driver Xi)
target_link_libraries(driver X11)
target_link_libraries(driver Xxf86vm)
target_link_libraries(driver Xcursor)
target_link_libraries(driver Xinerama)
target_link_libraries(driver GLEW)
target_link_libraries(driver GL)