cmake_minimum_required(VERSION 3.25)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

add_compile_options("-g")

set(PROJECT_CONFIG_DIR ${CMAKE_CURRENT_LIST_DIR})
include(../cmake/version.cmake) # -> VERSION_SEM
project(firment_test  VERSION ${VERSION_SEM} LANGUAGES CXX C)


# used in example test, may not be needed.
# set(CPPUTEST_INC_DIR ${PROJECT_SOURCE_DIR}/cpputest/include) 
include(FetchContent)
FetchContent_Declare(
    CppUTest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG v4.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(CppUTest)


# add_subdirectory(${PORT_DIR} port) # ??? Does firment link against port
add_subdirectory(.. firment)

add_executable(testExecutable
  src/testMain.cpp
)

target_link_libraries(testExecutable
  CppUTest
  firment-base
)
