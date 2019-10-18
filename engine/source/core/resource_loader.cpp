#include "core/resource_loader.h"



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

// Specializations

template <>
sf::Texture ResourceLoader::load<sf::Texture>(std::string name) const {
    const auto path = make_path<sf::Texture>(name);
    engine_info("Loading resource: {}", path.filename().string());
    sf::Texture res;
    res.loadFromFile(path);
    return res;
}

template <>
std::unique_ptr<sf::Texture> ResourceLoader::load_ptr<sf::Texture>(
    std::string name) const {
    const auto path = make_path<sf::Texture>(name);
    engine_info("Loading resource: {}", path.filename().string());
    auto res = std::make_unique<sf::Texture>();
    res->loadFromFile(path);
    return res;
}

template <>
sf::Font ResourceLoader::load<sf::Font>(std::string name) const {
    const auto path = make_path<sf::Font>(name);
    engine_info("Loading resource: {}", path.filename().string());
    sf::Font res;
    res.loadFromFile(path);
    return res;
}

template <>
std::unique_ptr<sf::Font> ResourceLoader::load_ptr<sf::Font>(
    std::string name) const {
    const auto path = make_path<sf::Font>(name);
    engine_info("Loading resource: {}", path.filename().string());
    auto res = std::make_unique<sf::Font>();
    res->loadFromFile(path);
    return res;
}