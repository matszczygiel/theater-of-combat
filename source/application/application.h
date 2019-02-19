#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class Application {
   public:
    virtual ~Application() = default;

    void run();

   protected:
    virtual void initialize()                         = 0;
    virtual void handle_event(const sf::Event& event) = 0;
    virtual void update()                             = 0;
    virtual void render()                             = 0;
    virtual void finalize()                           = 0;

    sf::RenderWindow _window;
    bool _running = false;
};