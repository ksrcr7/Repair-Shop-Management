cmake_minimum_required(VERSION 3.16)
project(RepairShopManagementSystem)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include (FetchContent) 
FetchContent_Declare(
    google_test
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP true
)

FetchContent_MakeAvailable(google_test)

enable_testing()

add_library(RepairShopManagementSystemLib
    src/Device.cpp
    src/StringUtils.cpp
)

target_include_directories(
    RepairShopManagementSystemLib
    PUBLIC include
)

add_executable(RepairShopManagementSystem
    src/main.cpp
)

target_link_libraries(
    RepairShopManagementSystem
    PRIVATE
    RepairShopManagementSystemLib
)

add_executable(RepairShopTests
    tests/DeviceTests.cpp
)

target_link_libraries(
    RepairShopTests
    PRIVATE
    RepairShopManagementSystemLib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(RepairShopTests)