#ifndef KIRCH_COMPONENTS_H
#define KIRCH_COMPONENTS_H

#include <cereal/types/base_class.hpp>

#include "toc/unit/unit_components.h"

namespace kirch {

using Movability = int;

struct MovementComponent : public ComponentBase {
    enum class FormationType { defence, attack, march };

    constexpr MovementComponent() = default;
    explicit MovementComponent(Movability moving_points);

    Movability moving_pts{};
    bool immobilized{false};
    FormationType formation{FormationType::march};

    constexpr Movability total_moving_pts() const noexcept { return _total_moving_pts; }

    template <class Archive>
    void serialize(Archive& archive);

   private:
    Movability _total_moving_pts{};
};

template <class Archive>
void MovementComponent::serialize(Archive& archive) {
    archive(cereal::base_class<ComponentBase>(this), CEREAL_NVP(_total_moving_pts),
            CEREAL_NVP(moving_pts), CEREAL_NVP(immobilized), CEREAL_NVP(formation));
}

using Strength = int;
struct DirectFightComponent : public ComponentBase {
    constexpr DirectFightComponent() = default;
    constexpr DirectFightComponent(Strength strength_points) noexcept
        : strength_pts{strength_points} {}

    Strength strength_pts{};
    bool in_fight{false};

    template <class Archive>
    void serialize(Archive& archive);
};

template <class Archive>
void DirectFightComponent::serialize(Archive& archive) {
    archive(cereal::base_class<ComponentBase>(this), CEREAL_NVP(strength_pts),
            CEREAL_NVP(in_fight));
}

struct DragoonComponent : public ComponentBase {
    constexpr DragoonComponent() = default;

    bool on_foot{false};

    template <class Archive>
    void serialize(Archive& archive);
};

template <class Archive>
void DragoonComponent::serialize(Archive& archive) {
    archive(cereal::base_class<ComponentBase>(this), CEREAL_NVP(on_foot));
}

}  // namespace kirch

#endif