cmake_minimum_required(VERSION 3.0)

# Set project's name
project(library C)

# Set project's language to C11
set(CMAKE_C_STANDARD 11)

# Adding source files in "src"
file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

# Adding header files in "include"
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Adding unit tests in "tests"
file(GLOB test_files "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.c")
foreach (file ${test_files})
    get_filename_component(file_name ${file} NAME_WLE)

    add_executable(${file_name} ${file})

    target_link_libraries(${file_name} ${PROJECT_NAME} m)
endforeach ()

# Adding an executable
add_executable(lib main.c)

target_link_libraries(lib ${PROJECT_NAME} m)

# Copy resource files from from data to build directory
file(GLOB resource_files "${CMAKE_CURRENT_SOURCE_DIR}/data/*")
foreach (item ${resource_files})
    FILE(COPY ${item} DESTINATION "${CMAKE_BINARY_DIR}")
endforeach ()