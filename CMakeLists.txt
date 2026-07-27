# EchoTerminal - Personal mood diary with retro terminal UI
# Supported platforms: Linux, Windows, macOS (via CMake)
# Build system: CMake 3.16+
# Qt version: 5.15.x (compatible with Qt 6.x)

cmake_minimum_required(VERSION 3.16)

project(EchoTerminal VERSION 1.0 LANGUAGES CXX)

# Включаем автоматическую обработку Qt-файлов
set(CMAKE_AUTOUIC ON)   # .ui файлы
set(CMAKE_AUTOMOC ON)   # макрос Q_OBJECT
set(CMAKE_AUTORCC ON)   # .qrc файлы ресурсов

# Стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск Qt (поддержка Qt5 и Qt6)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)

# Исходные файлы проекта
set(PROJECT_SOURCES
    main.cpp
    echoterminalwindow.cpp
    echoterminalwindow.h
    echoterminalwindow.ui
    resources.qrc
)

# Создание исполняемого файла
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(EchoTerminal
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(EchoTerminal SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(EchoTerminal
            ${PROJECT_SOURCES}
            .gitignore
            README.md
        )
    endif()
endif()

# Подключение библиотек Qt
target_link_libraries(EchoTerminal PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
)

# Свойства целевого объекта
set_target_properties(EchoTerminal PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.EchoTerminal
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Установка
include(GNUInstallDirs)
install(TARGETS EchoTerminal
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Финализация для Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(EchoTerminal)
endif()
