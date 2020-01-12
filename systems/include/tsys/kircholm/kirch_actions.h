#ifndef KIRCH_ACTIONS_H
#define KIRCH_ACTIONS_H

#include <cereal/types/vector.hpp>

#include "toc/gameplay/actions.h"

#include "kirch_fight.h"
#include "kirch_system.h"

namespace kirch {
class DirectFightResultsComputed : public SystemAction<SystemKircholm> {
   public:
    DirectFightResultsComputed() = default;
    explicit DirectFightResultsComputed(const std::vector<DirectFightResult>& results);
    bool sys_execute(SystemKircholm* system) override;
    bool sys_revert(SystemKircholm* system) override;

    template <class Archive>
    void serialize(Archive& ar);

   private:
    std::vector<DirectFightResult> _results{};
};

template <class Archive>
void DirectFightResultsComputed::serialize(Archive& ar) {
    ar(CEREAL_NVP(_results));
}

class DirectFightResultChanged : public SystemAction<SystemKircholm> {
   public:
    DirectFightResultChanged() = default;
    explicit DirectFightResultChanged(const std::vector<DirectFightResult>& results,
                                      int index);
    bool sys_execute(SystemKircholm* system) override;
    bool sys_revert(SystemKircholm* system) override;

    template <class Archive>
    void serialize(Archive& ar);

   private:
    DirectFightResult _result{};
    int _index{};
};

template <class Archive>
void DirectFightResultChanged::serialize(Archive& ar) {
    ar(CEREAL_NVP(_result), CEREAL_NVP(_index));
}

class RetretsPerformed : public SystemAction<SystemKircholm> {
   public:
    RetretsPerformed() = default;
    explicit RetretsPerformed(const std::vector<DirectFightResult>& results);
    bool sys_execute(SystemKircholm* system) override;
    bool sys_revert(SystemKircholm* system) override;

    template <class Archive>
    void serialize(Archive& ar);

   private:
    std::vector<DirectFightResult> _results{};
};

template <class Archive>
void RetretsPerformed::serialize(Archive& ar) {
    ar(CEREAL_NVP(_results));
}

class DirectFightChaseDone : public SystemAction<SystemKircholm> {
   public:
    DirectFightChaseDone() = default;
    explicit DirectFightChaseDone(int index);
    bool sys_execute(SystemKircholm* system) override;
    bool sys_revert(SystemKircholm* system) override;

    template <class Archive>
    void serialize(Archive& ar);

   private:
    int _index{};
};

template <class Archive>
void DirectFightChaseDone::serialize(Archive& ar) {
    ar(CEREAL_NVP(_index));
}

}  // namespace kirch

#endif /* KIRCH_ACTIONS_H */
