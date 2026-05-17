cmake_minimum_required(VERSION 3.19)
project(KUBYS LANGUAGES CXX)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)


qt_add_executable(KUBYS
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    varliklar.h
    depo.h
    kitapekledialog.h
    kitapekledialog.cpp
    kitapekledialog.ui
    kitapmodel.h
    kitapmodel.cpp
    uyemodel.h
    uyemodel.cpp
    uyeekledialog.h
    uyeekledialog.cpp
    uyeekledialog.ui
    oduncmodel.h oduncmodel.cpp
    oduncverdialog.h oduncverdialog.cpp oduncverdialog.ui
    .gitignore

)

target_link_libraries(KUBYS
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS KUBYS
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET KUBYS
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
