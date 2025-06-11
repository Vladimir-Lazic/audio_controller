add_library(${PROJECT_NAME}_lib
    audio/AudioController.cpp
    audio/WaveformGenerator.cpp
    threading/ThreadPool.cpp
    threading/Observer.h
    threading/Subject.h
    utils/Logger.h
    utils/TimeUtils.h)

target_include_directories(${PROJECT_NAME}_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/audio
    ${CMAKE_CURRENT_SOURCE_DIR}/threading
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

