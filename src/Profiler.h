#include "raylib.h"
#include <string>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <iostream>

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

    const std::vector<ProfileResult>& GetResults() const {
        return results;
    }

private:
    std::chrono::high_resolution_clock::time_point frameStart, frameEnd;
    std::vector<ProfileResult> results;
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

// Demo functions to profile
void SimulateWork(float ms) {
    auto start = std::chrono::high_resolution_clock::now();
    while (std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - start).count() < ms);
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "C++ Raylib Profiler Example");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Profiler::Get().BeginFrame();

        {
            ProfilerScope scope("Update");
            SimulateWork(5);  // Simulate a task
        }

        {
            ProfilerScope scope("Physics");
            SimulateWork(8);  // Simulate another task
        }

        {
            ProfilerScope scope("RenderPrep");
            SimulateWork(3);
        }

        Profiler::Get().EndFrame();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Profiler Output (ms)", 10, 10, 20, DARKGRAY);

        const auto& results = Profiler::Get().GetResults();
        int y = 40;
        for (const auto& result : results) {
            DrawText(result.name.c_str(), 10, y, 20, BLACK);
            DrawRectangle(150, y, result.timeMs * 10, 20, SKYBLUE); // bar scaled
            DrawText(TextFormat("%.2fms", result.timeMs), 160 + result.timeMs * 10, y, 20, DARKGRAY);
            y += 30;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
