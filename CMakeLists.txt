cmake_minimum_required(VERSION 3.20)

project(KUBYS VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

include_directories(.)

set(COMMON_SOURCES
    kitapmodel.cpp
    uyemodel.cpp
    oduncmodel.cpp
    kitapyukleyici.cpp
)

set(CLIENT_SOURCES
    main.cpp
    mainwindow.cpp
    kitapekledialog.cpp
    uyeekledialog.cpp
    oduncverdialog.cpp
    kaynaklar.qrc
    ${COMMON_SOURCES}
)

add_executable(KUBYS ${CLIENT_SOURCES})
target_link_libraries(KUBYS PRIVATE Qt6::Widgets Qt6::Network)

set(SERVER_SOURCES
    server_main.cpp
    ${COMMON_SOURCES}
)

add_executable(KUBYS_Server ${SERVER_SOURCES})
set_target_properties(KUBYS_Server PROPERTIES WIN32_EXECUTABLE FALSE)
target_link_libraries(KUBYS_Server PRIVATE Qt6::Network Qt6::Widgets)