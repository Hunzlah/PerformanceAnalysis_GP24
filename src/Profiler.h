#pragma once
#include "raylib.h"
#include <string>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <fstream>

// Struct to hold timing data
struct ProfileResult {
    std::string name;
    float timeMs;
};

// Singleton Profiler
class Profiler {
public:
    static Profiler& Get() {
        static Profiler instance;
        return instance;
    }

    void BeginFrame() {
        results.clear();
        frameStart = std::chrono::high_resolution_clock::now();
    }

    void EndFrame() {
        frameEnd = std::chrono::high_resolution_clock::now();
    }

    void AddResult(const std::string& name, float timeMs) {
        results.push_back({ name, timeMs });
    }
    void AddResultConstant(const std::string& name, float timeMs) {
        resultsConstant.push_back({ name, timeMs });
    }
    void EndSession(const std::string &filename)
    {
        std::ofstream ofs(filename);
        if (!ofs.is_open())
        {
            std::cerr << "Failed to open " << filename << std::endl;
            return;
        }

        for (const auto &entry : resultsConstant)
        {
            ofs << entry.name << ": " << entry.timeMs << " ms" << std::endl;
        }
        for (const auto &entry : results)
        {
            ofs << entry.name << ": " << entry.timeMs << " ms" << std::endl;
        }
    }
    const std::vector<ProfileResult>& GetResults() const {
        return results;
    }
    const std::vector<ProfileResult>& GetResultsConstant() const {
        return resultsConstant;
    }
private:
    std::chrono::high_resolution_clock::time_point frameStart, frameEnd;
    std::vector<ProfileResult> results;
    std::vector<ProfileResult> resultsConstant;
};

// RAII helper for scoped timing
class ProfilerScope {
public:
    ProfilerScope(const std::string& name)
        : name(name), start(std::chrono::high_resolution_clock::now()) {}

    ~ProfilerScope() {
        auto end = std::chrono::high_resolution_clock::now();
        float durationMs = std::chrono::duration<float, std::milli>(end - start).count();
        Profiler::Get().AddResult(name, durationMs);
    }

private:
    std::string name;
    std::chrono::high_resolution_clock::time_point start;
};
class ProfilerScopeConstant {
    public:
    ProfilerScopeConstant(const std::string& name)
            : name(name), start(std::chrono::high_resolution_clock::now()) {}
    
        ~ProfilerScopeConstant() {
            auto end = std::chrono::high_resolution_clock::now();
            float durationMs = std::chrono::duration<float, std::milli>(end - start).count();
            Profiler::Get().AddResultConstant(name, durationMs);
        }
    
    private:
        std::string name;
        std::chrono::high_resolution_clock::time_point start;
    };


