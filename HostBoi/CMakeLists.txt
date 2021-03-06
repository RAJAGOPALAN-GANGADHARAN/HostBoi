cmake_minimum_required(VERSION 3.14)

project(HostBoi LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network REQUIRED)

add_custom_target(copy-files ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory
   ${CMAKE_SOURCE_DIR}/static
   ${CMAKE_BINARY_DIR}/static
   )
add_executable(HostBoi
    enums.h
    handlers.h
    tcpthread.cpp
    controller.cpp
    main.cpp
)
target_link_libraries(HostBoi Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)
