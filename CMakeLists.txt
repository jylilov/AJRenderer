cmake_minimum_required(VERSION 2.8.4)
project(AJRenderer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXUTABLE_COMPILE_FLAGS}")

set(SOURCES
    main.cpp
    mainwindow.cpp
    renderer.cpp
    objectmodel.cpp
    texture.cpp
    vertex.cpp
    shader.cpp)

set(HEADERS
    mainwindow.h
    renderer.h
    vector.h
    objectmodel.h
    matrix.h
    texture.h
    vertex.h
    shader.h)

add_executable(AJRenderer ${SOURCES} ${HEADERS} vectorview.cpp vectorview.h)

target_link_libraries(AJRenderer ${Qt5Widgets_LIBRARIES})