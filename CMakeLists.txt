cmake_minimum_required(VERSION 3.30)
project(sdl_grid)

set(CMAKE_CXX_STANDARD 20)

# Find SDL3 and SDL3_image using CMake's find_package
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

# Include SDL3 and SDL3_image headers
include_directories(${SDL3_INCLUDE_DIRS} ${SDL3_IMAGE_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES "src/*.cpp")

# Add your source files
add_executable(${PROJECT_NAME} ${SOURCES})

# Link against SDL3 and SDL3_image libraries
target_link_libraries(${PROJECT_NAME} SDL3::SDL3 SDL3_image::SDL3_image)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets")
