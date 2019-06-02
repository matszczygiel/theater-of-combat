#pragma once

#include <cereal/types/base_class.hpp>
#include <cereal/types/vector.hpp>

class Message {
   public:
    virtual ~Message()       = default;
    int x                    = 2;
    virtual void log() const = 0;
    template <class Archive>
    void serialize(Archive& ar) { ar(CEREAL_NVP(x)); }
};

class Unit_move_request : public Message {
   public:
    int _unit_id = 1;

    virtual void log() const override;
    virtual void log2() const = 0;

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(cereal::virtual_base_class<Message>(this)),
           CEREAL_NVP(_unit_id));
    }
};

class Unit_move_request2 : public Unit_move_request {
   public:
    int _unit_id2 = 2;

    virtual void log2() const override;

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(cereal::virtual_base_class<Unit_move_request>(this)),
           CEREAL_NVP(_unit_id2));
    }
};