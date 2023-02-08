cmake_minimum_required(VERSION 3.10)

# set the project name
project(test)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


if (NOTIDY)
else ()
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=-*,clang-*,cppcoreguidelines-*,-cppcoreguidelines-avoid-magic-numbers,performance-*,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,llvm-*,modernize-*,-modernize-use-trailing-return-type,cert-*")
endif()

if (MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/Z7 /W4 /fsanitize=address")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /W4")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer -fvisibility=hidden -Wall -Wextra -pedantic -Werror -Wno-unused-parameter -Wno-unused-function -Wno-overloaded-virtual -Wshadow -Wconversion")

    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fvisibility=hidden -Wall -Wextra -pedantic -Werror -Wno-unused-parameter -Wno-unused-function -Wno-overloaded-virtual -Wshadow -Wconversion")
endif()

include_directories(HashConsing)

# add the executable
add_executable(test main.cpp HashConsing/HashFunction.hh HashConsing/HashTable.hh)


