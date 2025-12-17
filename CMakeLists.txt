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

# Save original compiler flags
set(ORIGINAL_CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(ORIGINAL_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Set compiler flags for GLFW (suppress warnings)
if(NOT MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
endif()

# Configure GLFW build options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Add GLFW as a subdirectory
add_subdirectory(third_party/glfw)

# Restore original compiler flags for our code
set(CMAKE_C_FLAGS "${ORIGINAL_CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${ORIGINAL_CMAKE_CXX_FLAGS}")

# Set compiler flags for our code
if(MSVC)
    # Microsoft Visual C++ compiler
    add_compile_options(/W4)

    # Debug specific flags
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/Zi /DEBUG /Od)
        add_link_options(/DEBUG)
    endif()
    
    # Release specific flags
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(/O2 /Ob2 /DNDEBUG)
    endif()
else()
    # GCC/Clang compiler
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter)

    # Debug specific flags
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g3 -O0 -D_GLIBCXX_DEBUG)
        add_link_options(-g3)
    endif()
    
    # Release specific flags
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O3 -DNDEBUG)
    endif()
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
    file(GLOB_RECURSE ${COMPONENT}_HEADERS "src/${COMPONENT}/*.hpp")
    list(APPEND ALL_SOURCES ${${COMPONENT}_SOURCES})
    list(APPEND ALL_HEADERS ${${COMPONENT}_HEADERS})
endforeach()

# Add include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include
)

# Create executable
add_executable(${PROJECT_NAME} ${MAIN_SOURCE} ${ALL_SOURCES} ${ALL_HEADERS})

# Add target include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    $<$<BOOL:${MSVC}>:stdc++fs>
    $<$<BOOL:${WIN32}>:opengl32>
    $<$<NOT:$<BOOL:${WIN32}>>:GL>
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

# Add stb_image
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb/stb_image.h
     DESTINATION ${CMAKE_BINARY_DIR}/third_party/stb)

# Add GLAD source files
file(GLOB GLAD_SOURCES "third_party/glad/src/*.c")
target_sources(${PROJECT_NAME} PRIVATE ${GLAD_SOURCES})
