cmake_minimum_required(VERSION 3.5)

project(nilcc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(QT NAMES Qt6 Qt5 COMPONENTS Charts QuickWidgets Widgets LinguistTools REQUIRED)
# find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Charts QuickWidgets Widgets LinguistTools REQUIRED)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_define_targets()

# set(TS_FILES nilcc_ru_RU.ts)

# set(PROJECT_SOURCES
#         conc.cpp
#         conc.h
#         controller.cpp
#         controller.h
#         main.cpp
#         mainwindow.cpp
#         mainwindow.h
#         qeigenmatrixmodel.cpp
#         qeigenmatrixmodel.h
#         qml.qrc
#         tablemodel.cpp
#         tablemodel.h
#         ${TS_FILES}
# )

# if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#     qt_add_executable(nilcc
#         MANUAL_FINALIZATION
#         ${PROJECT_SOURCES}
#     )
# # Define target properties for Android with Qt 6 as:
# #    set_property(TARGET nilcc APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
# #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

#     qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
# else()
#     if(ANDROID)
#         add_library(nilcc SHARED
#             ${PROJECT_SOURCES}
#         )
# # Define properties for Android with Qt 5 after find_package() calls as:
# #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#     else()
#         add_executable(nilcc
#             ${PROJECT_SOURCES}
#         )
#     endif()

#     qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
# endif()

# target_link_libraries(nilcc
#     PRIVATE CONAN_PKG::eigen
#     PRIVATE Qt${QT_VERSION_MAJOR}::Charts
#     PRIVATE Qt${QT_VERSION_MAJOR}::QuickWidgets
#     PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# set_target_properties(nilcc PROPERTIES
#     MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
#     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
# )

# if(QT_VERSION_MAJOR EQUAL 6)
#     qt_finalize_executable(nilcc)
# endif()

# file(COPY main.qml DESTINATION ${CMAKE_BINARY_DIR})

add_executable(test-conc
  test-conc.cpp
  conc.cpp
  conc.h)

target_link_libraries(test-conc
  CONAN_PKG::eigen)

add_executable(test-stb
  conc.cpp
  conc.h
  matrix.h
  stb.h
  test-stb.cpp
  )

target_link_libraries(test-stb
  CONAN_PKG::backward-cpp
  CONAN_PKG::ctre
  CONAN_PKG::eigen
  CONAN_PKG::repr
  CONAN_PKG::tomlplusplus
  )
