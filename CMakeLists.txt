cmake_minimum_required(VERSION 3.19)
project(BankAccountSystem LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(BankAccountSystem
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    bankaccount.h bankaccount.cpp
    createaccountdialog.h createaccountdialog.cpp createaccountdialog.ui
    .gitignore
)

qt_add_translations(
    TARGETS BankAccountSystem
    TS_FILES BankAccountSystem_en_BI.ts
)

target_link_libraries(BankAccountSystem
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS BankAccountSystem
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET BankAccountSystem
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
