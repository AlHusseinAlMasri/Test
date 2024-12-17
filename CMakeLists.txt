cmake_minimum_required(VERSION 3.16)

project(untitled5 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Widgets)

qt_standard_project_setup(REQUIRES 6.5)

# Include your header files
include_directories(${CMAKE_SOURCE_DIR}/include)

# Link the directory containing the DLL and LIB
link_directories(${CMAKE_SOURCE_DIR}/lib)


qt_add_executable(appuntitled5
    main.cpp
    lib/prof_dp.dll #the DLL name
)

qt_add_qml_module(appuntitled5
    URI untitled5
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES settingshandler.h settingshandler.cpp
        SOURCES
        QML_FILES Second.qml
        QML_FILES
        QML_FILES
        RESOURCES res.qrc
        QML_FILES ButtenTest.qml
)
# Copy the DLL file to the output directory (runtime path)
add_custom_command(TARGET appuntitled5 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/lib/prof_dp.dll  # Path to your DLL
        $<TARGET_FILE_DIR:appuntitled5>          # Output directory
)

# Search for the library (replace 'mylibrary' with your library name)
find_library(MY_LIBRARY
    NAMES prof_dp        # Name of the library to find (without extension)
    PATHS ${CMAKE_SOURCE_DIR}/lib   # Directory to search
    NO_DEFAULT_PATH          # Avoid system directories
)

# Check if the library was found
if (MY_LIBRARY)
    message(STATUS "Library found: ${MY_LIBRARY}")
else()
    message(WARNING "Al Hussein: Library not found in ${LIBRARY_PATH}")
endif()
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appuntitled5 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appuntitled5
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appuntitled5
    PRIVATE Qt6::Quick Qt6::Widgets
)

include(GNUInstallDirs)
install(TARGETS appuntitled5
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
