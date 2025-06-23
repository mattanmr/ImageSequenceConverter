# Minimal CMakeLists.txt for tests
cmake_minimum_required(VERSION 3.16)
enable_testing()
find_package(Qt6 REQUIRED COMPONENTS Test Widgets)

add_executable(test_presetmanager test_presetmanager.cpp ../src/presetmanager.cpp ../src/constants.h)
target_link_libraries(test_presetmanager Qt6::Test Qt6::Widgets)
target_include_directories(test_presetmanager PRIVATE ../src)
add_test(NAME PresetManagerTest COMMAND test_presetmanager)
