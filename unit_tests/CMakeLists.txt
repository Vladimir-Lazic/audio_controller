file(GLOB TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/threading/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp
)

add_executable(unit_tests ${TEST_SOURCES})

target_link_libraries(unit_tests gtest_main audio_controller_lib)
