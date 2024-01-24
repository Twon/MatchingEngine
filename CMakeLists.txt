cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

if(NOT DEFINED PROJECT_NAME)
    set(MATCHING_ENGINE_IS_NOT_SUBPROJECT ON)
endif()

project(MatchingEngine
    DESCRIPTION "A simple example matching engine and tests"
    LANGUAGES CXX
)

include(CTest)
include(FetchContent)
include(GNUInstallDirs)

add_library(MatchingEngine INTERFACE)
target_include_directories(MatchingEngine
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features(MatchingEngine
    INTERFACE   
        cxx_std_23
)
target_sources(MatchingEngine
    INTERFACE
        matching_engine.hpp
        types.hpp
)

if (${MATCHING_ENGINE_IS_NOT_SUBPROJECT} AND ${BUILD_TESTING})
    FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.2
    )
    FetchContent_MakeAvailable(catch2)

    add_executable(MatchingEngineTests)
    target_sources(MatchingEngineTests
        PRIVATE
            matching_engine.tests.cpp
    )
    target_link_libraries(MatchingEngineTests
        PRIVATE
            MatchingEngine
            Catch2::Catch2WithMain
    )
    set_target_properties(MatchingEngineTests PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )

    enable_testing()
    add_test(
        NAME MatchingEngineTests
        COMMAND MatchingEngineTests
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    include(Catch)
    catch_discover_tests(MatchingEngineTests)

endif (${MATCHING_ENGINE_IS_NOT_SUBPROJECT} AND ${BUILD_TESTING})