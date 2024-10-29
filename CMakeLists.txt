# 设置 CMake 的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(SocketProject)

# 为调试模式启用调试信息
set(CMAKE_C_STANDARD 99) # 设置C标准为C99
set(CMAKE_C_FLAGS_DEBUG "-g")

# 添加源文件
set(SOCKET_HELPER_SRC socket_helper.c)

# 为客户端和服务器端各自生成可执行文件
add_executable(client client.c ${SOCKET_HELPER_SRC})
add_executable(server server.c)

# 设置头文件路径，让client.c能够找到socket_helper.h
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
