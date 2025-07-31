#include "plot_window.hpp"
#include <implot.h>

void RenderPlots(const std::vector<PlotConfig>& configs,
                 const std::map<std::string, CSVData>& loadedCSVs) {
    for (const auto& cfg : configs) {
        const auto& data = loadedCSVs.at(cfg.csv_path);
        if (ImPlot::BeginPlot(cfg.title.c_str())) {
            auto it = data.metrics.find(cfg.metric);
            if (it != data.metrics.end()) {
                ImPlot::PlotLine(cfg.metric.c_str(), data.time.data(),
                                 it->second.data(), data.time.size());
                if (cfg.show_points) {
                    ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
                    ImPlot::PlotScatter((cfg.metric + "_pts").c_str(),
                                        data.time.data(), it->second.data(), data.time.size());
                }
            }
            ImPlot::EndPlot();
        }
    }
}
