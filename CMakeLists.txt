cmake_minimum_required(VERSION 3.10)
project(audio_controller LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(GTest REQUIRED)
find_package(RtAudio REQUIRED)

include_directories(${RTAUDIO_INCLUDE_DIR})

#add_subdirectory(unit_tests)
add_subdirectory(src)

add_executable(${PROJECT_NAME}
  src/main.cpp
)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib ${RTAUDIO_LIBRARY} asound pulse)


