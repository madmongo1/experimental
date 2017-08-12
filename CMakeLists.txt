cmake_minimum_required(VERSION 3.8)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(HunterGate)
include(Report)

HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.19.67.tar.gz"
        SHA1 "cd5ee033ae3949400781a3cf1bd79b79fba24f94"
)

project(experimental)
hunter_add_package(Sugar)
include(${SUGAR_ROOT}/cmake/Sugar)
include(sugar_files)
include(sugar_include)

add_custom_target(everything ALL)

set(CMAKE_CXX_STANDARD 14)

sugar_include(include)
sugar_include(src)

add_library(experimental ${SOURCE_FILES} ${PUBLIC_HEADERS})
target_include_directories(experimental SYSTEM PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
add_dependencies(everything experimental)

find_package(Doxygen QUIET)
if (DOXYGEN_FOUND)
    include(sugar_doxygen_generate)
    sugar_doxygen_generate(DEVELOPER TARGET experimental DOXYTARGET docs DOXYFILE ${SUGAR_ROOT}/examples/DoxyFile.in)
    add_dependencies(everything docs)
    if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
        add_custom_target(view_docs
                DEPENDS docs
                VERBATIM
                COMMAND "open" "${CMAKE_CURRENT_BINARY_DIR}/doxygen-docs/doxygen/html/index.html")
    endif ()
endif ()

option(EXPERIMENTAL_FORCE_TESTING "force the building of tests" ON)
if (EXPERIMENTAL_FORCE_TESTING)
    hunter_add_package(GTest)
endif ()

find_package(GTest CONFIG QUIET)
if (TARGET GTest::gtest)
    enable_testing()
    sugar_include(tests)
    add_executable(experimental-test-suite ${TEST_SOURCES})
    target_link_libraries(experimental-test-suite GTest::main experimental)
    add_test(ExperimentalTests experimental-test-suite)
endif ()


report(CMAKE_HOST_SYSTEM_NAME)
report(CMAKE_HOST_SYSTEM_VERSION)
report(CMAKE_HOST_SYSTEM_PROCESSOR)
report(CMAKE_HOST_SYSTEM)

