cmake_minimum_required(VERSION 3.10)   # 要求 CMake 最低版本
project(MyProject)                     # 项目名

set(CMAKE_CXX_STANDARD 17)             # 使用 C++17

add_executable(myapp src/main.cpp)    # 指定生成的可执行文件和源文件

