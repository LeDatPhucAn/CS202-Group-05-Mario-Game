# set(CMAKE_C_COMPILER gcc)
# set(CMAKE_CXX_COMPILER g++)

cmake_minimum_required(VERSION 3.10)
project(ProjectCS202)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include(FetchContent)  # Include module FetchContent to download raylib
# FetchContent_Declare(raylib
#     GIT_REPOSITORY https://github.com/raysan5/raylib.git
#     GIT_TAG 5.0
# )
# FetchContent_MakeAvailable(raylib)

set(RAYLIB_PATH "C:/raylib")

include_directories(${RAYLIB_PATH}/include)
link_directories(${RAYLIB_PATH}/lib)

include_directories(include)

# Đệ quy tìm tất cả các file header trong thư mục header
file(GLOB_RECURSE HEADER_FILES "header/*.h" "header/*.hpp")
foreach(header ${HEADER_FILES})
    get_filename_component(header_dir ${header} DIRECTORY)
    include_directories(${header_dir})
endforeach()

file(GLOB_RECURSE SOURCES "source/*.cpp" "header/*.cpp")  # Đệ quy tìm trong các thư mục con

add_executable(main ${SOURCES})

add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_BINARY_DIR}/assets
)

target_link_libraries(main raylib -lopengl32 -lgdi32 -lwinmm)

target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/header)