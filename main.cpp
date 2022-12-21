
#include <cassert>
#include <chrono>
#include <iostream>
#include <map>
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

auto main() -> int
{
    int N = 1000;
    speedtest1(N);
    speedtest2(N);
    speedtest3(N);
    speedtest4(N);
    speedtest5(N);

    return 0;
}