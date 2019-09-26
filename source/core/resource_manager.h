#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <typeindex>

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>

#include "log.h"

namespace fs = std::filesystem;

class ResourceManager {
   public:
    explicit ResourceManager(std::string resources_path);

    template <class Resource>
    void save_json(Resource& res, std::string name) const;

    template <class Resource>
    Resource load_json(std::string name) const;

    template <class Resource>
    void save_bin(Resource& res, std::string name) const;

    template <class Resource>
    Resource load_bin(std::string name) const;

    template <class Resource>
    void register_resource_type(std::string subdirectory, std::string postfix);

    template <class Resource>
    bool is_registered() const;

    const fs::path& resources_path() const;

   private:
    template <class Resource>
    fs::path make_path_json(std::string name) const;

    template <class Resource>
    fs::path make_path_bin(std::string name) const;

    fs::path _resources_path{};

    std::map<std::type_index, std::pair<std::string, std::string>>
        _type_dirs_and_postfixes{};
};

template <class Resource>
void ResourceManager::save_json(Resource& res, std::string name) const {
    static_assert(std::is_default_constructible<Resource>::value,
                  "Resource type must implement cereal serialization.");

    const auto path = make_path_json<Resource>(name);
    engine_info("Saving resource: {}", path.filename().string());
    std::ofstream file(path);
    cereal::JSONOutputArchive ar(file);
    ar(res);
}

template <class Resource>
Resource ResourceManager::load_json(std::string name) const {
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
void ResourceManager::save_json(Resource& res, std::string name) const {
    static_assert(std::is_default_constructible<Resource>::value,
                  "Resource type must implement cereal serialization.");

    const auto path = make_path_json<Resource>(name);
    engine_info("Saving resource: {}", path.filename().string());
    std::ofstream file(path);
    cereal::PortableBinaryOutputArchive ar(file);
    ar(res);
}

template <class Resource>
Resource ResourceManager::load_bin(std::string name) const {
    static_assert(std::is_default_constructible<Resource>::value,
                  "Resource type must implement cereal serialization.");

    const auto path = make_path_bin<Resource>(name);
    engine_info("Loading resource: {}", path.filename().string());
    std::ifstream file(path);
    cereal::PortableBinaryInputArchive ar(file);
    Resource res;
    ar(res);
    return res;
}

template <class Resource>
void ResourceManager::register_resource_type(std::string subdirectory,
                                             std::string postfix) {
    static_assert(std::is_default_constructible<Resource>::value,
                  "Resource type must implement cereal serialization.");
    _type_dirs_and_postfixes[std::type_index(typeid(Resource))] = {subdirectory,
                                                                   postfix};
}

template <class Resource>
fs::path ResourceManager::make_path_json(std::string name) const {
    auto path = _resources_path;
    auto search =
        _type_dirs_and_postfixes.find(std::type_index(typeid(Resource)));
    if (search != _type_dirs_and_postfixes.end()) {
        path += "/" + search->second.first + "/" + name + "-" +
                search->second.second;
    } else {
        path += name;
    }
    path += ".json";
    return path;
}

template <class Resource>
bool ResourceManager::is_registered() const {
    return _type_dirs_and_postfixes.find(std::type_index(typeid(Resource))) ! =
               _type_dirs_and_postfixes.end();
}

template <class Resource>
fs::path ResourceManager::make_path_bin(std::string name) const {
    auto path = _resources_path;
    auto search =
        _type_dirs_and_postfixes.find(std::type_index(typeid(Resource)));
    if (search != _type_dirs_and_postfixes.end()) {
        path += "/" + search->second.first + "/" + name + "." +
                search->second.second;
    } else {
        path += name + ".bin";
    }
    return path;
}

#endif