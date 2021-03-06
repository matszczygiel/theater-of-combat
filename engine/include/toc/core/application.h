#ifndef APPLICATION_H
#define APPLICATION_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class Application {
   public:
    virtual ~Application() = default;

    void run();

   protected:
    Application() noexcept;

    virtual void initialize()                         = 0;
    virtual void update(const sf::Time& elapsed_time) = 0;
    virtual void render()                             = 0;
    virtual void clear_loop()                         = 0;
    virtual void finalize()                           = 0;

    virtual void key_pressed_event(const sf::Keyboard::Key& key)                    = 0;
    virtual void key_released_event(const sf::Keyboard::Key& key)                   = 0;
    virtual void mouse_button_pressed_event(const sf::Mouse::Button& button,
                                            const sf::Vector2f& position)           = 0;
    virtual void mouse_button_released_event(const sf::Mouse::Button& button,
                                             const sf::Vector2f& position)          = 0;
    virtual void mouse_wheel_scrolled_event(const float& delta)                     = 0;
    virtual void window_resize_event(const unsigned& width, const unsigned& height) = 0;
    virtual void mouse_moved_event(const sf::Vector2f& position)                    = 0;

    bool imgui_want_mouse();
    bool imgui_want_keyboard();

    sf::RenderWindow _window{};

    bool _running{false};

   private:
    void handle_event(const sf::Event& event);
};

#endif