/* Copyright 2023 Yann ORLAREY
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cassert>
#include <chrono>
#include <concepts>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace HashConsing
{

// Check if T has a std::hash<T>{}(a) function
template <typename T>
concept hasHashFunction = requires(T a) {
                              {
                                  std::hash<T>{}(a)
                                  } -> std::convertible_to<std::size_t>;
                          };

// Check if T has a == and != operator
template <typename T>
concept hasEqualOperator = requires(T a, T b) {
                               {
                                   a == b
                                   } -> std::convertible_to<bool>;
                               {
                                   a != b
                                   } -> std::convertible_to<bool>;
                           };

/**
 * @brief If T has a hash function, use it, otherwise provides a default hash function that only work
 * for flat structures, i.e. structures that can be represented as an array of integers.
 */
template <typename T, typename R = unsigned int>
struct HashFunction {
    static std::size_t combine(std::size_t x, std::size_t y) { return (x > y) ? x * x + x + y : x + y * y; }

    std::size_t operator()(T const& s) const noexcept
    {
        if constexpr (hasHashFunction<T>) {
            return std::hash<T>{}(s);
        } else {
            // check at compilet time that T is a multiple of size_t
            constexpr std::size_t n = sizeof(T) / sizeof(R);
            static_assert(0 == (sizeof(T) % sizeof(R)));

            auto* p = reinterpret_cast<const R*>(&s);

            std::size_t seed = *p++;
            for (unsigned int i = 1; i < n; ++i) {
                seed = combine(seed, *p++);
            }
            return seed;
        }
    }
};

template <typename T>
struct HashFunction<std::set<T>> {
    static std::size_t combine(std::size_t x, std::size_t y) { return (x > y) ? x * x + x + y : x + y * y; }

    std::size_t operator()(std::set<T> const& s) const noexcept
    {
        std::size_t seed = 0;
        for (auto& e : s) {
            seed = combine(seed, HashFunction<T>{}(e));
        }
        return seed;
    }
};

template <typename T>
struct HashFunction<std::vector<T>> {
    static std::size_t combine(std::size_t x, std::size_t y) { return (x > y) ? x * x + x + y : x + y * y; }

    std::size_t operator()(std::vector<T> const& s) const noexcept
    {
        std::size_t seed = 0;
        for (auto& e : s) {
            seed = combine(seed, HashFunction<T>{}(e));
        }
        return seed;
    }
};

template <typename T1, typename T2>
struct HashFunction<std::pair<T1, T2>> {
    static std::size_t combine(std::size_t x, std::size_t y) { return (x > y) ? x * x + x + y : x + y * y; }

    std::size_t operator()(std::pair<T1, T2> const& s) const noexcept
    {
        std::size_t seed = HashFunction<T1>{}(s.first);
        seed             = combine(seed, HashFunction<T2>{}(s.second));
        return seed;
    }
};

/**
 * @brief If T has a == and != operator, use it, otherwise provides a default equality function that only work
 * for flat structures, i.e. structures that can be represented as an array of integers.
 */
template <typename T, typename R = unsigned int>
struct Equality {
    bool operator()(const T& x, const T& y) const noexcept
    {
        if constexpr (hasEqualOperator<T>) {
            return x == y;
        } else {
            constexpr std::size_t n = sizeof(T) / sizeof(R);
            static_assert(0 == (sizeof(T) % sizeof(R)));
            auto* p = reinterpret_cast<const R*>(&x);
            auto* q = reinterpret_cast<const R*>(&y);
            for (unsigned int i = 0; i < n; ++i) {
                if (*p++ != *q++) return false;
            }
            return true;
        }
    }
};

}  // namespace HashConsing