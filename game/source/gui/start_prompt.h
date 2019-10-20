#ifndef START_PROMPT_H
#define START_PROMPT_H

#include <memory>
#include <string>

#include "networking/net_manager.h"

class StartPrompt {
   public:
    StartPrompt(std::shared_ptr<NetManager>& ptr) noexcept;

    void show_window();

   private:
    enum class ConnectionType { none, server, client };
    enum class Stage { none, setup, waiting, selecting };

    void show_setup_page();
    void show_waiting_page();

    std::shared_ptr<NetManager> _nm{nullptr};

    std::string _player_name{};
    ConnectionType _connection_t{ConnectionType::none};
    int _server_port{0};
    std::string _server_ip{};
    Stage _stage{Stage::setup};
};

#endif