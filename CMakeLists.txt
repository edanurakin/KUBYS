cmake_minimum_required(VERSION 3.20)

project(KUBYS VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

include_directories(.)

# ------------------------------------------------------------------
# 1. İSTEMCİ (ARAYÜZ) İÇİN GEREKLİ DOSYALAR (KUBYS)
# ------------------------------------------------------------------
set(CLIENT_HEADERS
    mainwindow.h
    kitapmodel.h
    uyemodel.h
    oduncmodel.h
    kitapekledialog.h
    uyeekledialog.h
    oduncverdialog.h
    depo.h
    varliklar.h
)

set(CLIENT_UI
    mainwindow.ui
    kitapekledialog.ui
    uyeekledialog.ui
    oduncverdialog.ui
)

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
    ${CLIENT_HEADERS}
    ${CLIENT_UI}
)

add_executable(KUBYS ${CLIENT_SOURCES})
target_link_libraries(KUBYS PRIVATE Qt6::Widgets Qt6::Network)


# ------------------------------------------------------------------
# 2. SUNUCU (SERVER) İÇİN GEREKLİ DOSYALAR (KUBYS_Server)
# ------------------------------------------------------------------
# Sunucunun arayüz pencerelerine (.ui ve .h) ihtiyacı yoktur!
set(SERVER_HEADERS
    depo.h
    varliklar.h
)

set(SERVER_SOURCES
    server_main.cpp
    ${COMMON_SOURCES}
    ${SERVER_HEADERS}
)

add_executable(KUBYS_Server ${SERVER_SOURCES})
set_target_properties(KUBYS_Server PROPERTIES WIN32_EXECUTABLE FALSE)
target_link_libraries(KUBYS_Server PRIVATE Qt6::Network Qt6::Widgets)