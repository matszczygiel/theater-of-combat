#include "resource_loader.h"

ResourceLoader::ResourceLoader(std::string resources_path)
    : _resources_path{resources_path} {
    engine_info("Creating ResourceManager with path: {}", _resources_path.string());
    engine_assert_throw(fs::is_directory(_resources_path), "Path: {} is not a directory",
                        _resources_path.string());

    if (!fs::exists(_resources_path)) {
        engine_assert_throw(fs::create_directories(_resources_path),
                            "Could not create directories.");
    }
}

const fs::path& ResourceLoader::resources_path() const { return _resources_path; }
