cmake_minimum_required(VERSION 3.16)

project(proyecto5 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(proyecto5
    main.cpp
    configuracion.cpp
    configuracion.h
    jugador.cpp
    jugador.h
    Obstaculo.cpp
    Obstaculo.h
    Particula.cpp
    Particula.h
    Simulador.cpp
    Simulador.h
    mainwindow.h mainwindow.cpp mainwindow.ui
)

target_link_libraries(proyecto5 PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

target_compile_definitions(proyecto5 PRIVATE
    PROJECT_SOURCE_DIR_MACRO="${CMAKE_CURRENT_SOURCE_DIR}"
)