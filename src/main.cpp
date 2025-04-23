#include "Engine.hpp"
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    try {
        // Get the engine instance
        auto& engine = ShadowEngine::Engine::GetInstance();

        // Initialize the engine with default window parameters
        if (!engine.Initialize("Shadow Engine", 1280, 720)) {
            std::cerr << "Failed to initialize engine!" << std::endl;
            return 1;
        }

        // Run the engine
        engine.Run();

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
} 