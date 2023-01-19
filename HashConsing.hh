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

#include <concepts>
#include <cstdlib>
#include <memory>
#include <unordered_map>

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
struct DefaultHashFunction {
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

/**
 * @brief If T has a == and != operator, use it, otherwise provides a default equality function that only work
 * for flat structures, i.e. structures that can be represented as an array of integers.
 */
template <typename T, typename R = unsigned int>
struct DefaultEquality {
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

/**
 * HashTable<T> : T -> T const *
 */
template <typename T, typename Hash = DefaultHashFunction<T>, typename Equal = DefaultEquality<T>>
class HashTable
{
    /**
     * The internal hash table
     */
    std::unordered_map<T, std::unique_ptr<T>, Hash, Equal> fHashTable;

   public:
    /**
     * The hash consing operator: T -> T const *
     */
    auto operator()(const T& i) -> T const*
    {
        auto p = fHashTable.find(i);
        if (p != fHashTable.end()) {
            return p->second.get();
        }
        fHashTable.insert({i, std::make_unique<T>(i)});
        auto q = fHashTable.find(i);
        return q->second.get();
    }

    /**
     * Reports collision and other information on the internal hash table. Useful for debugging and evaluation of the
     * hash function
     */
    void report()
    {
        std::map<unsigned int, unsigned int> distribution;
        size_t                               collisions = 0, empty = 0;
        for (auto bucket = fHashTable.bucket_count(); bucket--;) {
            distribution[fHashTable.bucket_size(bucket)] += 1;
            if (fHashTable.bucket_size(bucket) == 0)
                empty++;
            else
                collisions += fHashTable.bucket_size(bucket) - 1;
        }
        std::cout << "size " << fHashTable.size() << ", max load factor " << fHashTable.max_load_factor()
                  << ", load factor " << fHashTable.load_factor() << ", bucket count " << fHashTable.bucket_count()
                  << ", collisions " << collisions << ", empty buckets " << empty << '\n';
        // distribution :
        for (auto [n, c] : distribution) {
            std::cout << '(' << n << '=' << c << ") ";
        }
        std::cout << std::endl;
    }
};

}  // namespace HashConsing