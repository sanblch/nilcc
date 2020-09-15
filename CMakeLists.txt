cmake_minimum_required(VERSION 3.12)

project(nilcc)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 5.15 COMPONENTS Quick Widgets)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_define_targets()

add_executable(${PROJECT_NAME}
  conc.cpp
  conc.h
  controller.cpp
  controller.h
  main.cpp
  qml.qrc
  tablemodel.cpp
  tablemodel.h
  )

target_include_directories(${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
  CONAN_PKG::eigen
  Qt5::Quick
  Qt5::Widgets)

add_executable(test-conc
  test-conc.cpp
  conc.cpp
  conc.h)

target_link_libraries(test-conc
  CONAN_PKG::eigen)
