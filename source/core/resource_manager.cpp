#include "resource_manager.h"


ResourceManager::ResourceManager(std::string resources_path)
    : _resources_path{resources_path} {
    engine_info("Creating ResourceManager with path: {}", _resources_path.string());
    engine_assert(fs::is_directory(_resources_path),
                  "Path: {} is not a directory", _resources_path.string());

    if (!fs::exists(_resources_path)) {
        engine_assert(fs::create_directories(_resources_path),
                      "Could not create directories.");
    }
}
