cmake_minimum_required(VERSION 3.10)
project(ShadowEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Set compiler flags
if(MSVC)
    # Microsoft Visual C++ compiler
    add_compile_options(/W4)
else()
    # GCC/Clang compiler
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Define engine components
set(ENGINE_COMPONENTS
    core
    rendering
    physics
    audio
    scene
    math
    input
)

# Add source files for each component
foreach(COMPONENT ${ENGINE_COMPONENTS})
    file(GLOB_RECURSE ${COMPONENT}_SOURCES "src/${COMPONENT}/*.cpp")
    file(GLOB_RECURSE ${COMPONENT}_HEADERS "src/${COMPONENT}/*.h")
    list(APPEND ALL_SOURCES ${${COMPONENT}_SOURCES})
    list(APPEND ALL_HEADERS ${${COMPONENT}_HEADERS})
endforeach()

# Create executable
add_executable(${PROJECT_NAME} ${ALL_SOURCES} ${ALL_HEADERS})

# Add include directories
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add shader directory
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
)

# Add asset directory
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
)
