#include "graphics/unit_gfx.h"

#include "core/log.h"
#include "unit/unit_components.h"

constexpr static float pi                  = 3.14159265358979323846f;
constexpr static float directionless_angle = pi / 2.0f;

UnitGfx::UnitGfx(std::shared_ptr<Layout>& layout) noexcept : _layout{layout} {}

void UnitGfx::setup(UnitManager& manager, std::string texture_path,
                    const std::map<Unit::IdType, sf::IntRect>& texture_positions) {
    _all_tokens.clear();

    const auto loaded = _texture.loadFromFile(texture_path);
    engine_assert_throw(loaded, "Cannot load texture from file: {}", texture_path);

    _texture.setSmooth(true);
    for (const auto& [id, unit] : manager.units()) {
        if (auto it = texture_positions.find(id);
            it != texture_positions.end() && loaded) {
            _all_tokens.emplace(id, Token(_layout, HexCoordinate::origin(),
                                          directionless_angle, &_texture, it->second));
        } else {
            _all_tokens.emplace(id, Token(_layout, HexCoordinate::origin(),
                                          directionless_angle, nullptr, sf::IntRect()));
        }
    }
    update(manager);
}

void UnitGfx::update(UnitManager& manager) {
    clear();
    for (const auto& [id, unit] : manager.units()) {
        const auto pos_cmp = manager.get_component<PositionComponent>(id);
        if (!pos_cmp)
            continue;

        const auto pos = pos_cmp->position;
        if (!pos)
            continue;

        const auto dir = pos_cmp->direction;

        if (!dir)
            _all_tokens.at(id).update(*pos, directionless_angle);
        else
            _all_tokens.at(id).update(*pos, _layout->direction_angle(*dir));

        tokens.emplace_back(std::make_pair(id, _all_tokens.at(id)));
    }
}

void UnitGfx::draw_tokens(sf::RenderTarget& target) const {
    for (const auto& [id, shape] : tokens) {
        target.draw(shape.shape());
    }
}

void UnitGfx::draw_ids(sf::RenderTarget& target, const sf::Font& font) const {
    static const sf::Color dark_magenta(139, 0, 139);

    for (const auto& [id, token] : tokens) {
        std::string text_str = "Id: " + std::to_string(id);
        sf::Text text{text_str, font, static_cast<unsigned int>(_layout->size.x * 0.25)};

        text.setFillColor(dark_magenta);
        text.setOutlineColor(dark_magenta);
        text.setStyle(sf::Text::Bold);
        text.setOrigin(text.getLocalBounds().width / 2.0, text.getLocalBounds().height);
        text.setPosition(token.shape().getPosition());
        target.draw(text);
    }
}

void UnitGfx::clear() noexcept { tokens.clear(); }