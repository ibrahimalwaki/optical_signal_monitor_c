cmake_minimum_required(VERSION 3.16)
project(optical_signal_monitor_c C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror -O2 -g)

add_executable(optical_monitor
    src/main.c
    src/acquisition.c
    src/processing.c
    src/queue.c
)

find_package(Threads REQUIRED)
target_link_libraries(optical_monitor PRIVATE Threads::Threads m)

target_include_directories(optical_monitor PRIVATE include)
