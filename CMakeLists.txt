cmake_minimum_required(VERSION 3.0)
project(cli-quiz)

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE cpr::cpr)
target_link_libraries(main PRIVATE nlohmann_json nlohmann_json::nlohmann_json)