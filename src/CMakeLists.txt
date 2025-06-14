add_library(${PROJECT_NAME}_lib
    audio/AudioController.cpp
    audio/WaveformGenerator.cpp
    observer/SocketObserver.cpp
    observer/ConsoleObserver.cpp
    threading/ThreadPool.cpp
    observer/Observer.h
    observer/Subject.h
    utils/Logger.h
    utils/Utils.h)

target_include_directories(${PROJECT_NAME}_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/audio
    ${CMAKE_CURRENT_SOURCE_DIR}/observer
    ${CMAKE_CURRENT_SOURCE_DIR}/threading
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

