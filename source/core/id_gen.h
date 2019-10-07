#ifndef ID_GEN_H
#define ID_GEN_H

#include <algorithm>
#include <type_traits>
#include <vector>

template <typename T>
class IdGenerator {
    static_assert(std::is_integral_v<T>, "Id must be an integral type");

   public:
    explicit IdGenerator(T first_id = 0) noexcept : _id{first_id} {}

    T fetch() noexcept;
    void return_to_poll(T id);

   private:
    T _id{0};
    std::vector<T> _free_poll{};
};

template <typename T>
T IdGenerator<T>::fetch() noexcept {
    if (_free_poll.empty())
        return _id++;
    else {
        const auto id = _free_poll.back();
        _free_poll.pop_back();
        return id;
    }
}

template <typename T>
void IdGenerator<T>::return_to_poll(T id) {
    if (std::none_of(std::cbegin(_free_poll), std::cend(_free_poll),
                     [&id](const auto& poll_id) { return poll_id == id; }))
        _free_poll.push_back(id);
}

#endif