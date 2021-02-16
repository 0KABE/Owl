#pragma once

#include <type_traits>
#include <vector>
#include <array>

namespace Owl {
    template<typename T>
    concept IsVector = std::is_same_v<T, std::vector<typename T::value_type, typename T::allocator_type>>;

    template<typename T>
    concept IsArray = std::is_same_v<T, std::array<typename T::value_type, std::tuple_size_v<T>>>;

    template<typename T>
    concept IsBuiltInArray = std::is_array_v<T>;

    template<typename T>
    concept IsArrayOrVector = IsVector<T> || IsArray<T> || IsBuiltInArray<T>;
}