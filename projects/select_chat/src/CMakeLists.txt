cmake_minimum_required(VERSION 3.10)
project(select_chat)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件，src/main.cpp 是源文件
add_executable(target server.c client.c)

# 指定头文件目录，给编译器用
target_include_directories(target PRIVATE ${PROJECT_SOURCE_DIR}/../include)




#通过 target_include_directories 指明头文件目录为项目根目录下的 include。

# executing in the root directory 
#cmake -S src -B build
#cmake --build build
#./build/my_app
