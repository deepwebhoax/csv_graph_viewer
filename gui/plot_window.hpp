#pragma once
#include "types.hpp"
#include <map>

void RenderPlots(const std::vector<PlotConfig>& configs,
                 const std::map<std::string, CSVData>& loadedCSVs);
