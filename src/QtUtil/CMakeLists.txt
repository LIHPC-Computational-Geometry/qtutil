set (CURRENT_PACKAGE_NAME "QtUtil")
set (CURRENT_PACKAGE_MACRO_NAME "QT_UTIL")

find_package (GUIToolkitsVariables)

include (${CMAKE_SOURCE_DIR}/cmake/version.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/common.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/common_qt.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/workarounds.cmake)

find_package (TkUtil 6 REQUIRED)
find_package (Qt${QT_MAJOR}Widgets ${QT_MAJOR} REQUIRED NO_CMAKE_SYSTEM_PATH)
find_package (Qt${QT_MAJOR}PrintSupport ${QT_MAJOR} REQUIRED NO_CMAKE_SYSTEM_PATH)
#
# Aide hypertexte avec navigateur web. Le navigateur utilisant sur le QtWebEngine qui repose sur Chromium est nettement mieux
# que le QtTextBrowser, mais son installation est très délicate et incertaine ...
# L'utilisateur peut imposer son choix à l'aide des options cmake suivantes :
# - USE_QT_WEB_ENGINE	: utilisation de l'API Qt avec moteur chromium (solution la plus évoluée mais pas la plus simple à installer)
# - USE_QT_WEBKIT		: utilisation de l'ancienne API Qt (< Qt 5.6) forkée et disponible sous license GPL v 2.1 sur internet (https://github.com/qt/qtwebkit)
# - USE_QT_TEXT_BROWSER	: utilisation du browser Qt (solution la moins évoluée)
# En l'absence de spécification utilisateur on essaie de charger le QtWebEngine. Si ça ne marche pas on se rabat sur le QtWebKit, puis sur le QtTextBrowser.
#
if ((NOT USE_QT_WEB_ENGINE) AND (NOT USE_QT_WEBKIT) AND (NOT USE_QT_TEXT_BROWSER))
	message (STATUS "--> Aide hypertexte : tentative d'utiliser QtWebEngine (meilleur) ...")
	find_package (Qt${QT_MAJOR}WebEngineWidgets ${QT_MAJOR} NO_CMAKE_SYSTEM_PATH)
	if (Qt${QT_MAJOR}WebEngineWidgets_FOUND)
		message (STATUS "--> QtWebEngine trouvé et utilisé.")
		set (USE_QT_WEB_ENGINE ON)
	else (Qt${QT_MAJOR}WebEngineWidgets_FOUND)
		message (STATUS "--> QtWebEngine non trouvé : tentative d'utiliser QtWebKit en remplacement ...")
		find_package (Qt${QT_MAJOR}WebKitWidgets NO_CMAKE_SYSTEM_PATH)
		if (Qt${QT_MAJOR}WebKitWidgets_FOUND)
			message (STATUS "--> QtWebKit trouvé et utilisé.")
			set (USE_QT_WEBKIT ON)
		else (Qt${QT_MAJOR}WebKitWidgets_FOUND)
			message (STATUS "--> QtWebKit non trouvé : utilisation par défaut de QtTextBrowser pour l'aide hypertexte.")
			set (USE_QT_TEXT_BROWSER ON)
		endif (Qt${QT_MAJOR}WebKitWidgets_FOUND)
	endif (Qt${QT_MAJOR}WebEngineWidgets_FOUND)
endif ((NOT USE_QT_WEB_ENGINE) AND (NOT USE_QT_WEBKIT) AND (NOT USE_QT_TEXT_BROWSER))

if (USE_QT_WEB_ENGINE)
	find_package (Qt${QT_MAJOR}WebView ${QT_MAJOR} REQUIRED NO_CMAKE_SYSTEM_PATH)
	find_package (Qt${QT_MAJOR}WebEngineWidgets ${QT_MAJOR} REQUIRED NO_CMAKE_SYSTEM_PATH)
endif (USE_QT_WEB_ENGINE)
if (USE_QT_WEBKIT)
	find_package (Qt${QT_MAJOR}WebKitWidgets REQUIRED NO_CMAKE_SYSTEM_PATH)
endif (USE_QT_WEBKIT)

file (GLOB HEADERS public/${CURRENT_PACKAGE_NAME}/*.h)
file (GLOB CPP_SOURCES *.cpp)

# On ajoute les en-têtes aux sources. C'est utile pour cmake dans certains cas,
# par exemple lorsqu'ils doivent être pré-processés (moc, ...).
add_library (QtUtil ${CPP_SOURCES} ${HEADERS})
set_property (TARGET QtUtil PROPERTY AUTOMOC ON)
set (ALL_TARGETS QtUtil)
set_property (TARGET QtUtil PROPERTY VERSION ${QT_UTIL_VERSION})
set_property (TARGET QtUtil PROPERTY SOVERSION ${QT_UTIL_MAJOR_VERSION})

set (QT_UTIL_PUBLIC_FLAGS -DQT_${QT_MAJOR})
set (QT_LIB_DEPENDENCIES Qt${QT_MAJOR}::Widgets Qt${QT_MAJOR}::Gui Qt${QT_MAJOR}::PrintSupport Qt${QT_MAJOR}::Core)
set (QT_INC_DEPENDENCIES ${Qt${QT_MAJOR}Widgets_INCLUDE_DIRS} ${Qt${QT_MAJOR}Gui_INCLUDE_DIRS} ${Qt${QT_MAJOR}PrintSupport_INCLUDE_DIRS} ${Qt${QT_MAJOR}Core_INCLUDE_DIRS})
set (QtWebEngine_ENABLED OFF)	# Pour find_dependcy
set (QtWebKit_ENABLED OFF)	# Pour find_dependcy
if (USE_QT_TEXT_BROWSER)
	message (STATUS "==> Utilisation de QtTextBrowser pour l'aide hypertexte.")
	list (APPEND QT_UTIL_PUBLIC_FLAGS -DUSE_QT_TEXT_BROWSER)
else (USE_QT_TEXT_BROWSER)	# => QtWebEngine
	if (USE_QT_WEBKIT)
		message (STATUS "==> Utilisation de QtWebKit")
		set (QtWebKit_ENABLED ON)
		list (APPEND QT_UTIL_PUBLIC_FLAGS -DUSE_QT_WEBKIT)
		set (QT_LIB_DEPENDENCIES Qt${QT_MAJOR}::WebKitWidgets)
	else (USE_QT_WEBKIT)
		message (STATUS "==> Utilisation de QtWebEngine")
		set (QtWebEngine_ENABLED ON)
		list (APPEND QT_UTIL_PUBLIC_FLAGS -DUSE_QT_WEBENGINE)
		set (QT_LIB_DEPENDENCIES Qt${QT_MAJOR}::WebEngineWidgets Qt${QT_MAJOR}::WebView)
		set (QT_INC_DEPENDENCIES ${Qt${QT_MAJOR}WebEngineWidgets_INCLUDE_DIRS} ${Qt${QT_MAJOR}WebView_INCLUDE_DIRS})
	endif (USE_QT_WEBKIT)
endif (USE_QT_TEXT_BROWSER)
set (QT_UTIL_PRIVATE_FLAGS -DQT_UTIL_VERSION="${QT_UTIL_VERSION}")

target_include_directories (QtUtil PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)
target_compile_definitions (QtUtil PUBLIC ${QT_UTIL_PUBLIC_FLAGS})
target_compile_definitions (QtUtil PRIVATE ${QT_UTIL_PRIVATE_FLAGS})
target_compile_options (QtUtil PRIVATE ${SHARED_CFLAGS})	# Requested by Qt ...
target_include_directories (QtUtil PUBLIC ${QT_INC_DEPENDENCIES})
target_link_libraries (QtUtil PUBLIC TkUtil::TkUtil ${QT_LIB_DEPENDENCIES})
# Etre capable une fois installée de retrouver TkUtil, Qt${QT_MAJOR}*, ... :
# (Rem : en son absence on a Set runtime path of "/tmp/pignerol/install/lib/libQtUtil.so.5.0.0" to "") ...
set_target_properties (QtUtil PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1)
# Inefficace ici : CMAKE_AUTOMOC => dans le CMakeLists.txt racine
#set (CMAKE_AUTOMOC ON)

# INSTALLATION :
include(CMakePackageConfigHelpers)
# Génération du fichier contenant les informations sur la distribution :
configure_file (cmake/QtUtilDistribution.h.in  ${CMAKE_CURRENT_BINARY_DIR}/QtUtilDistribution.tmp)
configure_file (${CMAKE_CURRENT_BINARY_DIR}/QtUtilDistribution.tmp  ${CMAKE_CURRENT_BINARY_DIR}/QtUtilDistribution.h)

# ConfigPackageLocation : c'est plus ou moins standardisé, le défaut étant lib/cmake.
# On nos recommande très vivement d'utiliser ce répertoire afin de limiter les soucis par la suite,
# notamment au niveau des vues.
set (ConfigPackageLocation ${CMAKE_CMAKE_DIR})

# La génération des règles d'installation :
install (TARGETS ${ALL_TARGETS} EXPORT ${CMAKE_PACKAGE_TARGETS_NAME}
         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
         INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Recopie des en-têtes dans le répertoire d'installation :
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_PACKAGE_NAME})
# Dont QtUtilDistribution.h généré plus haut :
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/QtUtilDistribution.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_PACKAGE_NAME})

# Le fichier contenant les informations de version de ce module :
write_basic_package_version_file(${CMAKE_PACKAGE_VERSION_FILE} VERSION ${QT_UTIL_VERSION} COMPATIBILITY SameMajorVersion)
export (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME} FILE ${CMAKE_PACKAGE_TARGETS_FILE} NAMESPACE ${CMAKE_MODULE_NAMESPACE})

# Utilisation par d'autres modules de QtUtil :
set (INCLUDE_INSTALL_DIR include/)				# QtUtil_INCLUDE_DIR avec QtUtilConfig.cmake.in
set (LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})	# QtUtil_LIB_DIR avec QtUtilConfig.cmake.in
#set (RUNTIME_INSTALL_DIR bin/)					# QtUtil_RUNTIME_DIR avec QtUtilConfig.cmake.in
# Création du fichier QtUtilConfig.cmake qui définira QtUtil_INCLUDE_DIR, QtUtil_LIB_DIR et QtUtil_BIN_DIR :
configure_package_config_file (cmake/${CMAKE_PACKAGE_CONFIG_NAME}.cmake.in ${CMAKE_PACKAGE_CONFIG_FILE}
	INSTALL_DESTINATION ${CMAKE_CMAKE_DIR}
	PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR)

# Copie du fichier QtUtilTargets.cmake dans prefix/lib/QtUtil/cmake :
install (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME}
	FILE ${CMAKE_PACKAGE_TARGETS_BASENAME} NAMESPACE ${CMAKE_MODULE_NAMESPACE}
	DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)
# Copie des fichiers QtUtilConfig.cmake et QtUtilConfigVersion.cmake dans prefix/lib/QtUtil/cmake :
install (FILES ${CMAKE_PACKAGE_CONFIG_FILE} ${CMAKE_PACKAGE_VERSION_FILE} DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)
