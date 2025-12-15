cmake_minimum_required(VERSION 3.20)

project(Seminar12 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++ -Wl,--allow-multiple-definition")


add_compile_options(-Wall -Wextra)

set(FLTK_ROOT "D:\\FLTK")
find_package(FLTK REQUIRED)

set(TARGET "The_minesweeper")

add_executable(
    ${TARGET}
    src/The_minesweeper_cpp/The_minesweeper_main_file.cpp
    src/The_minesweeper_cpp/Board.cpp
    src/The_minesweeper_cpp/MainWindow.cpp
    src/The_minesweeper_cpp/MenuWindow.cpp
    src/Graph_lib/Graph.cpp
    src/Graph_lib/GUI.cpp
    src/Graph_lib/Window.cpp
)

target_include_directories(${TARGET} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${FLTK_INCLUDE_DIR}
)

target_link_directories(${TARGET} PRIVATE
    ${FLTK_INCLUDE_DIR}/../lib
)

target_link_libraries(${TARGET} PRIVATE
    ${FLTK_LIBRARIES}
    fltk_jpeg
    fltk_png
    fltk_z
    gdiplus
)

add_custom_target(CopyResources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resources
    ${CMAKE_BINARY_DIR}/resources
    COMMENT "Copying resources into runtime output directory"
)

add_dependencies(${TARGET} CopyResources)

find_package(FLTK REQUIRED)
if(FLTK_FOUND)
    include_directories(${FLTK_INCLUDE_DIRS})
    set(FLTK_LIBS fltk fltk_images)
endif()