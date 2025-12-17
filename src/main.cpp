// Entry point for ShadowEngine-based applications.
// This is intentionally kept minimal as a clean starting point.

#include "Engine.hpp"

int main(int /*argc*/, char* /*argv*/[]) {
    auto& engine = ShadowEngine::Engine::GetInstance();

    // TODO: Customize window title, size, and other initialization parameters as needed.
    if (!engine.Initialize("Shadow Engine", 1280, 720)) {
        return 1;
    }

    // TODO: Set up your game/application state here (scenes, systems, etc.).

    engine.Run();
    return 0;
}