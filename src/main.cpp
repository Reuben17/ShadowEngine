#include "Engine.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        // Get the engine instance
        auto& engine = ShadowEngine::Engine::GetInstance();

        // Initialize the engine
        if (!engine.Initialize()) {
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