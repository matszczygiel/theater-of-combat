#include "tokenizable.h"

#include "log.h"

bool Tokenizable::_texture_loaded = false;
sf::Texture Tokenizable::_tokens_texture;

void Tokenizable::load_textures(const std::string& filename) {
    ENGINE_INFO("Loading tokens texture, from: {0}", filename);

    if (!_tokens_texture.loadFromFile(filename)) {
        ENGINE_ERROR("Failed to load textures.");
        throw std::runtime_error("Failed to load textures.");
    }

    _texture_loaded = true;
}

bool Tokenizable::token_contains(const sf::Vector2f& vec) const {
    return _token.getGlobalBounds().contains(vec);
}

void Tokenizable::set_token_postion(const sf::Vector2f& position) {
    _token.setPosition(position.x - 0.5 * _token.getSize().x,
                       position.y - 0.5 * _token.getSize().y);
}

sf::Vector2f Tokenizable::get_token_position() const {
    auto pos = _token.getPosition();
    pos.x += 0.5 * _token.getSize().x;
    pos.y += 0.5 * _token.getSize().y;
    return pos;
}

void Tokenizable::draw(sf::RenderTarget& target) const {
    target.draw(_token);
}

void Tokenizable::init_token(const float& size) {
    if (!_texture_loaded) {
        ENGINE_ERROR("Initializing token without textures.");
        throw std::runtime_error("Initializing token without textures.");
    }

    _token.setSize(sf::Vector2f(size, size));
    _token.setTexture(&_tokens_texture);
    const auto text_size = _tokens_texture.getSize();
    const int offset     = text_size.y * texture_offset();

    
    ENGINE_ASSERT(offset < static_cast<int>(text_size.x),
    "Token initialized with too much texture offset. offset: {0}, texture width: {1}", offset, text_size.x);

    sf::IntRect texture_rect(offset, 0, text_size.y, text_size.y);
    _token.setTextureRect(texture_rect);

    ENGINE_TRACE("Token intialized of size: {0}.", size);
}

const float& Tokenizable::get_token_size() const {
    return _token.getSize().x;
}

const sf::RectangleShape& Tokenizable::get_token() const {
    return _token;
}