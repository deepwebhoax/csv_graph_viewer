cmake_minimum_required(VERSION 3.15)
project(CSVGraphViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(SOURCES
    main.cpp
)

file(GLOB_RECURSE MODULE_SOURCES
    gui/*.cpp
    io/*.cpp
)

list(APPEND SOURCES ${MODULE_SOURCES})

# Define headers for IDEs
file(GLOB_RECURSE HEADERS
    include/*.hpp
    gui/*.hpp
    io/*.hpp
)

add_executable(csv_graph_viewer ${SOURCES} ${HEADERS})

# Dependencies
find_package(SDL3 REQUIRED)
find_package(nlohmann_json REQUIRED)

# Include directories
target_include_directories(csv_graph_viewer PRIVATE
    ${SDL3_INCLUDE_DIRS}
    include
    imgui
    imgui/backends
    implot
)

# ImGui and ImPlot sources
target_sources(csv_graph_viewer PRIVATE
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_demo.cpp
    imgui/backends/imgui_impl_sdl3.cpp
    imgui/backends/imgui_impl_sdlrenderer3.cpp
    implot/implot.cpp
    implot/implot_items.cpp
)

# Link libraries
target_link_libraries(csv_graph_viewer PRIVATE
    SDL3::SDL3
    nlohmann_json::nlohmann_json
)
