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

#include "HashFunction.hh"

namespace HashConsing
{

/**
 * HashTable<T> : T -> T const *
 */
template <typename T, typename Hash = HashFunction<T>, typename Equal = Equality<T>>
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
        std::map<size_t, size_t> distribution;
        size_t                   collisions = 0, empty = 0;
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