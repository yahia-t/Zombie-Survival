cmake_minimum_required(VERSION 3.10)
project(IdolZombieSurvival)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    include
    libs/glew/include
    libs/glut/include
    libs/assimp/include
    libs/glaux/include
    libs/stb
)

# Link directories
link_directories(
    libs/glew/lib
    libs/glut/lib
    libs/assimp/lib
    libs/glaux/lib
)

# Source files
set(SOURCES
    src/camera.cpp
    src/main.cpp
    src/model_loader.cpp
    src/player.cpp

)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} "include/zombie.h" "src/zombie.cpp" "include/projectile.h" "src/projectile.cpp" "include/powerup.h" "src/powerup.cpp")

# Link libraries
target_link_libraries(${PROJECT_NAME} glew32 glaux glut32 assimp-vc143-mt zlib opengl32)

# Platform-specific configurations
if(WIN32)
    add_definitions(-D_WINDOWS) # Optional Windows-specific macro
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} GL GLU X11) # Add Linux-specific libraries if needed
endif()

# Copy assets to the runtime directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

# Copy all required DLLs to the runtime directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/libs/glew/bin/glew32.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/libs/glut/bin/glut32.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/libs/assimp/bin/assimp-vc143-mt.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/libs/assimp/bin/poly2tri.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/libs/assimp/bin/draco.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/libs/assimp/bin/minizip.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/libs/assimp/bin/pugixml.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/libs/assimp/bin/zlib1.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/libs/glaux/bin/glaux.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# Remove redundant extra copy commands for `libs`
# Individual required DLLs are already being copied above.
