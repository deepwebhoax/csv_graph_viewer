# CSV Graph Viewer

A simple desktop application using SDL3 + ImGui + ImPlot to visualize CSV data.

## ✅ Features

- Load JSON configuration to determine plot layout.
- Plot CSV time-series data with ImPlot.
- Dockable GUI with ImGui.
- Customizable metrics and styles.

## 🔧 Build Instructions

```bash
git clone --recurse-submodules <repo>
cd csv_graph_viewer
mkdir build && cd build
cmake ..
make
./csv_graph_viewer
