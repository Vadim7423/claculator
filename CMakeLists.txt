cmake_minimum_required(VERSION 3.17.0 FATAL_ERROR)

project(calc VERSION 0.1 LANGUAGES CXX)

list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE_DIR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 COMPONENTS
                            Core
                            Gui
                            Qml
                            Quick
                            Concurrent
                  REQUIRED)

set(srcFiles
    main.cpp
    src/calculator.h
    src/calculator.cpp
    src/queue_requests.h
    src/queue_requests.cpp
    src/queue_results.h
    src/queue_results.cpp
    src/settings.h
    src/settings.cpp
)
set(resFiles
    qml.qrc
)

#--------------------
#--- Build binary ---
add_executable(${PROJECT_NAME} ${srcFiles} ${resFiles})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::Concurrent
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        $<$<CONFIG:Debug>:
            -O0
        >
        $<$<CONFIG:Release>:
            -O2
        >
        -flto
        -std=c++${CMAKE_CXX_STANDARD}
)

target_link_options(${PROJECT_NAME}
    PRIVATE
        -flto
)
