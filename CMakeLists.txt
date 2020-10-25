cmake_minimum_required(VERSION 3.12)

project(nilcc)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 5.15 COMPONENTS
    Charts
    QuickWidgets
    Widgets)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_define_targets()

add_executable(${PROJECT_NAME}
  # chartmodel.cpp
  # chartmodel.h
  conc.cpp
  conc.h
  controller.cpp
  controller.h
  main.cpp
  mainwindow.cpp
  mainwindow.h
  qeigenmatrixmodel.cpp
  qeigenmatrixmodel.h
  qml.qrc
  tablemodel.cpp
  tablemodel.h
  )

target_include_directories(${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
  CONAN_PKG::Eigen3
  Qt5::Charts
  Qt5::QuickWidgets
  Qt5::Widgets)

add_executable(test-conc
  test-conc.cpp
  conc.cpp
  conc.h)

target_link_libraries(test-conc
  CONAN_PKG::Eigen3)
