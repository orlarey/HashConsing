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
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "HashFunction.hh"
#include "HashTable.hh"

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
    std::cout << "\nspeedtest1" << std::endl;

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
    std::cout << "\nspeedtest2" << std::endl;

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
 * @brief Create and retrieve N*N hash consed intervals (i,j) and check that they are the same.
 * Here interals are more complex, with 4 fields.
 */
void speedtest2k(int N)
{
    std::cout << "\nspeedtest2k" << std::endl;

    HashConsing::HashTable<const Interval2> H;

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

    std::cout << "\nItv2k   - Creation : " << d1.count() << "ms; Retrieving : " << d2.count() << "ms" << std::endl;

    H.report();
}

/**
 * @brief Create and retrieve N*N hash consed tuples (i,j) and check that they are the same.
 * Here intervals are tuples instead of structs.
 */
void speedtest3(int N)
{
    std::cout << "\nspeedtest3" << std::endl;

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
 * @brief Create and retrieve N*N hash consed tuples (i,j) and check that they are the same.
 * Here intervals are tuples instead of structs.
 */
void speedtest3k(int N)
{
    std::cout << "\nspeedtest3k" << std::endl;

    // HashTable<std::tuple<int, int>, MyHash<std::tuple<int, int>>> H;
    HashConsing::HashTable<const std::tuple<int, int>> H;

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

    std::cout << "\nTuplek  - Creation : " << d1.count() << "ms; Retrieving : " << d2.count() << "ms" << std::endl;
    H.report();
}

/**
 * @brief Create and retrieve N*N hash consed strings "[i,j]" and check that they are the same
 */
void speedtest4(int N)
{
    std::cout << "\nspeedtest4" << std::endl;

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

#if 0
/**
 * @brief Create and retrieve N*N hash consed strings "[i,j]" and check that they are the same
 */
void speedtest4k(int N)
{
    std::cout << "\nspeedtest4k" << std::endl;

    HashConsing::HashTable<const std::string, std::hash<const std::string>, std::equal_to<>> H;

    auto t1 = std::chrono::system_clock::now();
    // creating
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            const std::string s = "[" + std::to_string(i) + "," + std::to_string(j) + "]";
            /*const auto* itv = */ H(s);
        }
    }
    auto t2 = std::chrono::system_clock::now();
    // retrieving
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            const std::string s   = "[" + std::to_string(i) + "," + std::to_string(j) + "]";
            const auto*       itv = H(s);
            assert(*itv == s);
        }
    }
    auto t3 = std::chrono::system_clock::now();
    auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    auto d2 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2);

    std::cout << "\nString4k - Creation : " << d1.count() << "ms; Retrieving : " << d2.count() << "ms" << std::endl;
    H.report();
}
#endif

/**
 * @brief Create and retrieve N*N hash consed strings "[i,j]" and check that they are the same.
 * Here we use the default hash function and comparator.
 */
void speedtest5(int N)
{
    std::cout << "\nspeedtest5" << std::endl;

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

#if 1
/**
 * @brief Create and retrieve N*N hash consed strings "[i,j]" and check that they are the same.
 * Here we use the default hash function and comparator.
 */
void speedtest5k(int N)
{
    std::cout << "\nspeedtest5k" << std::endl;

    HashConsing::HashTable<const std::string> H;

    auto t1 = std::chrono::system_clock::now();
    // creating
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            const std::string s = "[" + std::to_string(i) + "," + std::to_string(j) + "]";
            /*const auto* itv = */ H(s);
        }
    }
    auto t2 = std::chrono::system_clock::now();
    // retrieving
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            const std::string s   = "[" + std::to_string(i) + "," + std::to_string(j) + "]";
            const auto*       itv = H(s);
            assert(*itv == s);
        }
    }
    auto t3 = std::chrono::system_clock::now();
    auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    auto d2 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2);

    std::cout << "\nString5k - Creation : " << d1.count() << "ms; Retrieving : " << d2.count() << "ms" << std::endl;
    H.report();
}
#endif

/**
 * @brief Create and retrieve N*N hash consed sets {i,j} and check that they are the same.
 */
void speedtest6(int N)
{
    std::cout << "\nspeedtest6" << std::endl;

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
 * @brief Create and retrieve N*N hash consed sets {i,j} and check that they are the same.
 */
void speedtest6k(int N)
{
    std::cout << "\nspeedtest6k" << std::endl;

    // HashTable<std::tuple<int, int>, MyHash<std::tuple<int, int>>> H;
    HashConsing::HashTable<const std::set<int>> H;

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
            const std::set<int> s = {i, j};
            const auto*         p = H(s);
            assert(s == *p);
        }
    }
    auto t3 = std::chrono::system_clock::now();
    auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    auto d2 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2);

    std::cout << "\nConstSet- Creation : " << d1.count() << "ms; Retrieving : " << d2.count() << "ms" << std::endl;
    H.report();
}

/**
 * @brief Create and retrieve N*N hash consed vectors {i,j} and check that they are the same.
 */
void speedtest7(int N)
{
    std::cout << "\nspeedtest7" << std::endl;

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

/**
 * @brief Create and retrieve N*N hash consed vectors {i,j} and check that they are the same.
 */
void speedtest7k(int N)
{
    std::cout << "\nspeedtest7k" << std::endl;

    // HashTable<std::tuple<int, int>, MyHash<std::tuple<int, int>>> H;
    HashConsing::HashTable<const std::vector<int>> H;

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

    std::cout << "\nVectorK - Creation : " << d1.count() << "ms; Retrieving : " << d2.count() << "ms" << std::endl;
    H.report();
}

/**
 * @brief Create and retrieve N*N hash consed pairs (i,j) and check that they are the same.
 * Here intervals are pairs instead of structs.
 */
void speedtest8(int N)
{
    std::cout << "\nspeedtest8" << std::endl;
    // HashTable<std::tuple<int, int>, MyHash<std::tuple<int, int>>> H;
    HashConsing::HashTable<std::pair<int, int>> H;

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
            const auto* p = H({i, j});
            const auto* q = H({j, i});
            assert(p->first == i && p->second == j);
            if (i != j) assert(p != q);
        }
    }
    auto t3 = std::chrono::system_clock::now();
    auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    auto d2 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2);

    std::cout << "\nPair     - Creation : " << d1.count() << "ms; Retrieving : " << d2.count() << "ms" << std::endl;
    H.report();
}

/**
 * @brief Create and retrieve N*N hash consed pairs (i*j,set{i,j}) and check that they are the same.
 */
void speedtest9(int N)
{
    std::cout << "\nspeedtest9" << std::endl;
    // HashTable<std::tuple<int, int>, MyHash<std::tuple<int, int>>> H;
    HashConsing::HashTable<std::pair<int, std::set<int>>> H;

    auto t1 = std::chrono::system_clock::now();
    // creating
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            const auto* p = H({i * j, {i, j}});
            const auto* q = H({j * i, {j, i}});
            assert(p == q);
        }
    }
    auto t2 = std::chrono::system_clock::now();
    // retrieving
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            const auto* p = H({i * j, {i, j}});
            assert(p->first == (i * j));
            std::set<int> s{i, j};
            assert(s == p->second);
        }
    }
    auto t3 = std::chrono::system_clock::now();
    auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    auto d2 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2);

    std::cout << "\nPairSet  - Creation : " << d1.count() << "ms; Retrieving : " << d2.count() << "ms" << std::endl;
    H.report();
}

void speedtestA(int N)
{
    // HashTable<std::tuple<int, int>, MyHash<std::tuple<int, int>>> H;
    HashConsing::HashTable<std::set<int>> H;

    const auto* p0 = H({});
    const auto* p1 = H({1});
    const auto* p2 = H({1, 2});
    const auto* p3 = H({1, 2, 3});
    std::cout << "N" << N << " p0=" << p0 << " p1=" << p1 << " p2=" << p2 << " p3=" << p3 << std::endl;
}

auto main() -> int
{
    int N = 1000;
    speedtest1(N);
    speedtest2(N);
    speedtest2k(N);
    speedtest3(N);
    speedtest3k(N);
    speedtest4(N);
    // speedtest4k(N);
    speedtest5(N);
    speedtest5k(N);
    speedtest6(N);
    speedtest6k(N);
    speedtest7(N);
    speedtest7k(N);
    speedtest8(N);
    speedtest9(N);
    std::cout << "OK" << std::endl;
    // speedtestA(N);
    return 0;
}
