find_package (GUIToolkitsVariables)

include (${CMAKE_SOURCE_DIR}/cmake/version.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/common.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/workarounds.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/common_qt.cmake)

find_package (Qt${QT_MAJOR}Widgets ${QT_MAJOR} REQUIRED NO_CMAKE_SYSTEM_PATH)

add_executable (flow Flow.qrc window.cpp main.cpp)
set_property (TARGET flow PROPERTY AUTOMOC ON)
set_property (TARGET flow PROPERTY AUTORCC ON)

target_include_directories (flow PRIVATE ../QtUtil/public ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options (flow PRIVATE ${SHARED_CFLAGS}) # Requested by Qt ...
target_link_libraries (flow PUBLIC QtUtil)

# INSTALL_RPATH modifie le rpath pour les libs internes au projet :
set_target_properties (flow PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1 INSTALL_RPATH ${CMAKE_PACKAGE_RPATH_DIR})
