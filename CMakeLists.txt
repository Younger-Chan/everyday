cmake_minimum_required(VERSION 3.5)

project(everyday VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network LinguistTools Xml Multimedia Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network LinguistTools Xml Multimedia Svg)

set(TS_FILES lang/everyday_zh_CN.ts)

set(PROJECT_SOURCES
        source/main.cpp
        source/everyday.cpp
        source/sentence.cpp
        source/weather.cpp
        source/weibo.cpp
        source/baidu.cpp
        source/douyin.cpp
        source/zhihu.cpp
        source/bilibili.cpp
        source/jian.cpp
        source/wangyiy.cpp
        source/calendar.cpp
        source/todolist.cpp
        source/flowlayout.cpp
        source/setting.cpp
        source/todoRemind.cpp
        source/trayicon.cpp
        source/customtitlebar.cpp
        source/customtooltip.cpp
        include/everyday.h
        include/sentence.h
        include/weather.h
        include/weibo.h
        include/baidu.h
        include/douyin.h
        include/zhihu.h
        include/bilibili.h
        include/jian.h
        include/wangyiy.h
        include/calendar.h
        include/todolist.h
        include/flowlayout.h
        include/setting.h
        include/todoRemind.h
        include/trayicon.h
        include/customtitlebar.h
        include/customtooltip.h
        ui/everyday.ui
        ui/sentence.ui
        ui/weather.ui
        ui/weibo.ui
        ui/baidu.ui
        ui/douyin.ui
        ui/zhihu.ui
        ui/bilibili.ui
        ui/jian.ui
        ui/wangyiy.ui
        ui/calendar.ui
        ui/todolist.ui
        ui/setting.ui
        ui/todoRemind.ui
        ui/customtitlebar.ui
        ui/customtooltip.ui
        rc/icon.qrc
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(everyday
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET everyday APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

# 添加翻译文件更新支持
    qt_add_lupdate(everyday
        SOURCES ${PROJECT_SOURCES}
    )
    qt_add_translations(everyday
        ${CMAKE_SOURCE_DIR}/lang
        ${TS_FILES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(everyday SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(everyday
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(everyday PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::Svg
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.everyday)
endif()
set_target_properties(everyday PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS everyday
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(everyday)
endif()
