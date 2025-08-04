#include "plot_window.hpp"
#include <implot.h>

void RenderPlots(const std::vector<PlotConfig>& configs,
    const std::map<std::string, CSVData>& loadedCSVs) {
    // ── shared-axis state ──────────────────────────────────────────────────────
    static double shared_x_min = 0, shared_x_max = 0;
    static bool   have_limits = false;

    // ── render plots ───────────────────────────────────────────────────────────
    for (const auto& cfg : configs) {
        size_t i = 0;
        std::string win_name = cfg.title + "###" + cfg.metric;
        bool open = true;
        ImGui::Begin(win_name.c_str(), &open,
                     ImGuiWindowFlags_None);

        const auto& data = loadedCSVs.at(cfg.csv_path);
        if (!have_limits && !data.time.empty()) {
            shared_x_min       = data.time.front();
            shared_x_max       = data.time.back();
            have_limits = true;
        }
        if (ImPlot::BeginPlot(cfg.title.c_str())) {
            ImPlot::SetupAxisLinks(ImAxis_X1, &shared_x_min, &shared_x_max);
            
            if (auto it = data.metrics.find(cfg.metric); it != data.metrics.end()) {
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
        ImGui::End();
    }
}
