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

#include <cassert>
#include <chrono>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>

#include "HashConsing.hh"

struct Interval1 {
    int lo;
    int hi;
};

struct Interval2 {
    int lo;
    int hi;
    int precision;
    int extra;
};

/**
 * @brief Create and retrieve N*N hash consed intervals (i,j) and check that they are the same
 */
void speedtest1(int N)
{
    HashConsing::HashTable<Interval1> H;

    auto t1 = std::chrono::system_clock::now();
    // creating
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            /*const auto* itv =*/H({i, j});
        }
    }
    auto t2 = std::chrono::system_clock::now();
    // retrieving
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            const auto* itv = H({i, j});
            assert(itv->lo == i && itv->hi == j);
        }
    }
    auto t3 = std::chrono::system_clock::now();
    auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    auto d2 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2);

    std::cout << "\nItv (a) - Creation : " << d1.count() << "ms; Retrieving : " << d2.count() << "ms" << std::endl;

    H.report();
}

/**
 * @brief Create and retrieve N*N hash consed intervals (i,j) and check that they are the same.
 * Here interals are more complex, with 4 fields.
 */
void speedtest2(int N)
{
    HashConsing::HashTable<Interval2> H;

    auto t1 = std::chrono::system_clock::now();
    // creating
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            /*const auto* itv =*/H({i, j, i + j, i * j});
        }
    }
    auto t2 = std::chrono::system_clock::now();
    // retrieving
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            const auto* itv = H({i, j, i + j, i * j});
            assert(itv->lo == i && itv->hi == j);
        }
    }
    auto t3 = std::chrono::system_clock::now();
    auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    auto d2 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2);

    std::cout << "\nItv (2) - Creation : " << d1.count() << "ms; Retrieving : " << d2.count() << "ms" << std::endl;

    H.report();
}

/**
 * @brief Create and retrieve N*N hash consed tuples (i,j) and check that they are the same.
 * Here intervals are tuples instead of structs.
 */
void speedtest3(int N)
{
    // HashTable<std::tuple<int, int>, MyHash<std::tuple<int, int>>> H;
    HashConsing::HashTable<std::tuple<int, int>> H;

    auto t1 = std::chrono::system_clock::now();
    // creating
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            /*const auto* itv = */ H({i, j});
        }
    }
    auto t2 = std::chrono::system_clock::now();
    // retrieving
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            const auto* itv = H({i, j});
            assert(std::get<0>(*itv) == i && std::get<1>(*itv) == j);
        }
    }
    auto t3 = std::chrono::system_clock::now();
    auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    auto d2 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2);

    std::cout << "\nTuple   - Creation : " << d1.count() << "ms; Retrieving : " << d2.count() << "ms" << std::endl;
    H.report();
}

/**
 * @brief Create and retrieve N*N hash consed strings "[i,j]" and check that they are the same
 */
void speedtest4(int N)
{
    HashConsing::HashTable<std::string, std::hash<std::string>, std::equal_to<>> H;

    auto t1 = std::chrono::system_clock::now();
    // creating
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::string s = "[" + std::to_string(i) + "," + std::to_string(j) + "]";
            /*const auto* itv = */ H(s);
        }
    }
    auto t2 = std::chrono::system_clock::now();
    // retrieving
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::string s   = "[" + std::to_string(i) + "," + std::to_string(j) + "]";
            const auto* itv = H(s);
            assert(*itv == s);
        }
    }
    auto t3 = std::chrono::system_clock::now();
    auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    auto d2 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2);

    std::cout << "\nString4 - Creation : " << d1.count() << "ms; Retrieving : " << d2.count() << "ms" << std::endl;
    H.report();
}

/**
 * @brief Create and retrieve N*N hash consed strings "[i,j]" and check that they are the same.
 * Here we use the default hash function and comparator.
 */
void speedtest5(int N)
{
    HashConsing::HashTable<std::string> H;

    auto t1 = std::chrono::system_clock::now();
    // creating
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::string s = "[" + std::to_string(i) + "," + std::to_string(j) + "]";
            /*const auto* itv = */ H(s);
        }
    }
    auto t2 = std::chrono::system_clock::now();
    // retrieving
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::string s   = "[" + std::to_string(i) + "," + std::to_string(j) + "]";
            const auto* itv = H(s);
            assert(*itv == s);
        }
    }
    auto t3 = std::chrono::system_clock::now();
    auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    auto d2 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2);

    std::cout << "\nString5 - Creation : " << d1.count() << "ms; Retrieving : " << d2.count() << "ms" << std::endl;
    H.report();
}

/**
 * @brief Create and retrieve N*N hash consed sets {i,j} and check that they are the same.
 */
void speedtest6(int N)
{
    // HashTable<std::tuple<int, int>, MyHash<std::tuple<int, int>>> H;
    HashConsing::HashTable<std::set<int>> H;

    auto t1 = std::chrono::system_clock::now();
    // creating
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            /*const auto* itv = */ H({i, j});
        }
    }
    auto t2 = std::chrono::system_clock::now();
    // retrieving
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::set<int> s = {i, j};
            const auto*   p = H(s);
            assert(s == *p);
        }
    }
    auto t3 = std::chrono::system_clock::now();
    auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    auto d2 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2);

    std::cout << "\nSet     - Creation : " << d1.count() << "ms; Retrieving : " << d2.count() << "ms" << std::endl;
    H.report();
}

/**
 * @brief Create and retrieve N*N hash consed vectors {i,j} and check that they are the same.
 */
void speedtest7(int N)
{
    // HashTable<std::tuple<int, int>, MyHash<std::tuple<int, int>>> H;
    HashConsing::HashTable<std::vector<int>> H;

    auto t1 = std::chrono::system_clock::now();
    // creating
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            /*const auto* itv = */ H({i, j});
        }
    }
    auto t2 = std::chrono::system_clock::now();
    // retrieving
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::vector<int> s0 = {i, j};
            std::vector<int> s1 = {j, i};
            const auto*      p  = H(s0);
            assert(s0 == *p);
            if (i != j) assert(s1 != *p);
        }
    }
    auto t3 = std::chrono::system_clock::now();
    auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    auto d2 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2);

    std::cout << "\nVector  - Creation : " << d1.count() << "ms; Retrieving : " << d2.count() << "ms" << std::endl;
    H.report();
}

auto main() -> int
{
    int N = 1000;
    speedtest1(N);
    speedtest2(N);
    speedtest3(N);
    speedtest4(N);
    speedtest5(N);
    speedtest6(N);
    speedtest7(N);
    std::cout << "OK" << std::endl;
    return 0;
}
