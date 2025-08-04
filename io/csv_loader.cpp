// io/csv_loader.cpp
#include "csv_loader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
CSVData loadCSV(const std::string& path) {
    CSVData data;
    std::ifstream file(path);
    std::string line;
    std::vector<std::string> headers;

    if (!std::getline(file, line)) 
        return data;
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, ',')) 
        headers.push_back(token);
    for (const auto& h : headers) 
        data.metrics[h] = {};

    while (std::getline(file, line)) {
        std::stringstream lineStream(line);
        int col = 0;
        while (std::getline(lineStream, token, ',')) {
            double val = std::stod(token);
            data.metrics[headers[col++]].push_back(val);
        }
    }

    data.time = data.metrics["time"];

    std::vector<double> normalized_time;
    double base = data.time.front();
    for (double t : data.time)
        normalized_time.push_back(t - base);

    data.time = normalized_time;

    return data;
}
