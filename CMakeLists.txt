cmake_minimum_required(VERSION 3.10)

# set the project name
project(test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=-*,clang-*,cppcoreguidelines-*,-cppcoreguidelines-avoid-magic-numbers,performance-*,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-cppcoreguidelines-pro-bounds-pointer-arithmetic, -cppcoreguidelines-pro-type-reinterpret-cast, llvm-*,modernize-*,cert-*")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# add the executable
add_executable(test main.cpp HashConsing.hh)


