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

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<IF:$<BOOL:${WIN32}>,windows,linux>/$<IF:$<BOOL:${CMAKE_SIZEOF_VOID_P}>,x64,x86>/${CMAKE_BUILD_TYPE}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<IF:$<BOOL:${WIN32}>,windows,linux>/$<IF:$<BOOL:${CMAKE_SIZEOF_VOID_P}>,x64,x86>/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<IF:$<BOOL:${WIN32}>,windows,linux>/$<IF:$<BOOL:${CMAKE_SIZEOF_VOID_P}>,x64,x86>/${CMAKE_BUILD_TYPE}/lib)

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

# Add main source file
set(MAIN_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Add source files for each component
foreach(COMPONENT ${ENGINE_COMPONENTS})
    file(GLOB_RECURSE ${COMPONENT}_SOURCES "src/${COMPONENT}/*.cpp")
    file(GLOB_RECURSE ${COMPONENT}_HEADERS "src/${COMPONENT}/*.h")
    list(APPEND ALL_SOURCES ${${COMPONENT}_SOURCES})
    list(APPEND ALL_HEADERS ${${COMPONENT}_HEADERS})
endforeach()

# Create executable
add_executable(${PROJECT_NAME} ${MAIN_SOURCE} ${ALL_SOURCES} ${ALL_HEADERS})

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
