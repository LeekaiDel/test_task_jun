cmake_minimum_required(VERSION 3.2)
project(test_jun)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRCS  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/my_list.cpp)

# Основная программа
add_executable(my_prog ${SRCS})
target_include_directories(my_prog PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Тестовая программа для проверки вывода списка из выходного файла
add_executable(test_reader ${CMAKE_CURRENT_SOURCE_DIR}/test_serialization.cpp)
