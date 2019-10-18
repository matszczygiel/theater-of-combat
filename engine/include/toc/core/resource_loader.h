#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <typeindex>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>

#include "log.h"

namespace fs = std::filesystem;

class ResourceLoader {
   public:
    explicit ResourceLoader(std::string resources_path);

    template <class Resource>
    void save_json(const Resource& res, std::string name) const;

    template <class Resource>
    Resource load_json(std::string name) const;

    template <class Resource>
    std::unique_ptr<Resource> load_ptr_json(std::string name) const;

    template <class Resource>
    void save(const Resource& res, std::string name) const;

    template <class Resource>
    Resource load(std::string name) const;

    template <class Resource>
    std::unique_ptr<Resource> load_ptr(std::string name) const;

    template <class Resource>
    void register_resource_type(std::string subdirectory, std::string extension);

    template <class Resource>
    bool is_registered() const;

    const fs::path& resources_path() const;

   private:
    template <class Resource>
    fs::path make_path_json(std::string name) const;

    template <class Resource>
    fs::path make_path(std::string name) const;

    fs::path _resources_path{};

    std::map<std::type_index, std::pair<std::string, std::string>>
        _type_dirs_and_extensions{};
};

template <class Resource>
void ResourceLoader::save_json(const Resource& res, std::string name) const {
    static_assert(std::is_default_constructible<Resource>::value,
                  "Resource type must implement cereal serialization.");

    const auto path = make_path_json<Resource>(name);
    engine_info("Saving resource: {}", path.filename().string());
    std::ofstream file(path);
    cereal::JSONOutputArchive ar(file);
    ar(res);
}

template <class Resource>
Resource ResourceLoader::load_json(std::string name) const {
    static_assert(std::is_default_constructible<Resource>::value,
                  "Resource type must implement cereal serialization.");

    const auto path = make_path_json<Resource>(name);
    engine_info("Loading resource: {}", path.filename().string());
    std::ifstream file(path);
    cereal::JSONInputArchive ar(file);

    Resource res;
    ar(res);
    return res;
}

template <class Resource>
std::unique_ptr<Resource> ResourceLoader::load_ptr_json(std::string name) const {
    static_assert(std::is_default_constructible<Resource>::value,
                  "Resource type must implement cereal serialization.");

    const auto path = make_path_json<Resource>(name);
    engine_info("Loading resource: {}", path.filename().string());
    std::ifstream file(path);
    cereal::JSONInputArchive ar(file);

    auto res = std::make_unique<Resource>();
    ar(*res);
    return res;
}

template <class Resource>
void ResourceLoader::save(const Resource& res, std::string name) const {
    static_assert(std::is_default_constructible<Resource>::value,
                  "Resource type must implement cereal serialization.");

    const auto path = make_path<Resource>(name);
    engine_info("Saving resource: {}", path.filename().string());
    std::ofstream file(path);
    cereal::PortableBinaryOutputArchive ar(file);
    ar(res);
}

template <class Resource>
Resource ResourceLoader::load(std::string name) const {
    static_assert(std::is_default_constructible<Resource>::value,
                  "Resource type must implement cereal serialization.");

    const auto path = make_path<Resource>(name);
    engine_info("Loading resource: {}", path.filename().string());
    std::ifstream file(path);
    cereal::PortableBinaryInputArchive ar(file);
    Resource res;
    ar(res);
    return res;
}

template <class Resource>
std::unique_ptr<Resource> ResourceLoader::load_ptr(std::string name) const {
    static_assert(std::is_default_constructible<Resource>::value,
                  "Resource type must implement cereal serialization.");

    const auto path = make_path<Resource>(name);
    engine_info("Loading resource: {}", path.filename().string());
    std::ifstream file(path);
    cereal::PortableBinaryInputArchive ar(file);

    auto res = std::make_unique<Resource>();
    ar(*res);
    return res;
}

template <class Resource>
void ResourceLoader::register_resource_type(std::string subdirectory,
                                            std::string extension) {
    static_assert(std::is_default_constructible<Resource>::value,
                  "Resource type must implement cereal serialization.");
    _type_dirs_and_extensions[std::type_index(typeid(Resource))] = {subdirectory,
                                                                    extension};
}

template <class Resource>
fs::path ResourceLoader::make_path_json(std::string name) const {
    auto path = _resources_path;
    if (const auto search =
            _type_dirs_and_extensions.find(std::type_index(typeid(Resource)));
        search != _type_dirs_and_extensions.end()) {
        path += "/" + search->second.first + "/" + name + "-" + search->second.second;
    } else {
        path += name;
    }
    path += ".json";
    return path;
}

template <class Resource>
bool ResourceLoader::is_registered() const {
    const auto it = _type_dirs_and_extensions.find(std::type_index(typeid(Resource)));
    return it != _type_dirs_and_extensions.end();
}

template <class Resource>
fs::path ResourceLoader::make_path(std::string name) const {
    auto path = _resources_path;
    if (const auto search =
            _type_dirs_and_extensions.find(std::type_index(typeid(Resource)));
        search != _type_dirs_and_extensions.end()) {
        path += "/" + search->second.first + "/" + name + "." + search->second.second;
    } else {
        path += name + ".bin";
    }
    return path;
}

// Specializations

template <>
sf::Texture ResourceLoader::load<sf::Texture>(std::string name) const;

template <>
std::unique_ptr<sf::Texture> ResourceLoader::load_ptr<sf::Texture>(
    std::string name) const;

template <>
sf::Font ResourceLoader::load<sf::Font>(std::string name) const;

template <>
std::unique_ptr<sf::Font> ResourceLoader::load_ptr<sf::Font>(std::string name) const;

/*
template <class Resource>
void ResourceLoader::save(const Resource& res, std::string name) const {
    static_assert(std::is_default_constructible<Resource>::value,
                  "Resource type must implement cereal serialization.");

    const auto path = make_path<Resource>(name);
    engine_info("Saving resource: {}", path.filename().string());
    std::ofstream file(path);
    cereal::PortableBinaryOutputArchive ar(file);
    ar(res);
}

template <class Resource>
Resource ResourceLoader::load(std::string name) const {
    static_assert(std::is_default_constructible<Resource>::value,
                  "Resource type must implement cereal serialization.");

    const auto path = make_path<Resource>(name);
    engine_info("Loading resource: {}", path.filename().string());
    std::ifstream file(path);
    cereal::PortableBinaryInputArchive ar(file);
    Resource res;
    ar(res);
    return res;
}

template <class Resource>
std::unique_ptr<Resource> ResourceLoader::load_ptr(std::string name) const {
    static_assert(std::is_default_constructible<Resource>::value,
                  "Resource type must implement cereal serialization.");

    const auto path = make_path<Resource>(name);
    engine_info("Loading resource: {}", path.filename().string());
    std::ifstream file(path);
    cereal::PortableBinaryInputArchive ar(file);

    auto res = std::make_unique<Resource>();
    ar(*res);
    return res;
}
*/
#endif