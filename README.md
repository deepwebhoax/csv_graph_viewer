# CSV Graph Viewer

A simple desktop application using SDL3 + ImGui + ImPlot to visualize CSV data on 2D plots.

## Features

- Load JSON configuration to determine plot layout.
- Plot CSV time-series data with ImPlot.
- Dockable GUI with ImGui.
- Customizable metrics and styles.

## Required Files

1. **CSV File (`data/test.csv`)**
2. **Config File (`config.json`)**

## Build Instructions

```bash
git clone --recurse-submodules <repo>
cd csv_graph_viewer
mkdir build && cd build
cmake ..
cmake --build .
./csv_graph_viewer
```
