#ifndef START_PROMPT_H
#define START_PROMPT_H

#include <memory>
#include <string>

#include "toc/core/resource_loader.h"

#include "networking/net_manager.h"

class StartPrompt {
   public:
    StartPrompt(std::shared_ptr<NetManager>& ptr,
                std::shared_ptr<ResourceLoader>& loader) noexcept;

    void show_window();

   private:
    enum class ConnectionType { none, server, client };
    enum class Stage { none = 0, setup, waiting, selecting };

    void show_setup_page();
    void show_waiting_page();
    void show_selecting_page();

    std::shared_ptr<NetManager> _nm{nullptr};
    std::shared_ptr<ResourceLoader> _rl{nullptr};

    std::string _player_name{};
    ConnectionType _connection_t{ConnectionType::none};
    std::string _server_ip{Server::get_local_ip().toString()};
    Stage _stage{Stage::setup};
    std::vector<std::string> _scenarios{};
};

#endif