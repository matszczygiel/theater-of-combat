#pragma once

#include <SFML/Graphics.hpp>

class Tokenizable {
   public:
    static void load_textures(const std::string& filename);

    void set_token_postion(const sf::Vector2f& position);
    sf::Vector2f get_token_position() const;

    virtual void draw(sf::RenderTarget& target) const;
    bool token_contains(const sf::Vector2f& vec);

    virtual void init_token(const float& size);

   protected:
    virtual int texture_offset() const = 0;

   private:
    sf::RectangleShape _token;

    static sf::Texture _tokens_texture;
    static bool _texture_loaded;
};