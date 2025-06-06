# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


set(CPACK_BINARY_7Z "OFF")
set(CPACK_BINARY_IFW "OFF")
set(CPACK_BINARY_NSIS "ON")
set(CPACK_BINARY_NUGET "OFF")
set(CPACK_BINARY_WIX "OFF")
set(CPACK_BINARY_ZIP "OFF")
set(CPACK_BUILD_SOURCE_DIRS "F:/Invisible_Girl_Game;F:/Invisible_Girl_Game")
set(CPACK_CMAKE_GENERATOR "Visual Studio 17 2022")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "F:/CMake/share/cmake-3.24/Templates/CPack.GenericDescription.txt")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_SUMMARY "Invisible_Girl_Game built using CMake")
set(CPACK_DMG_SLA_USE_RESOURCE_FILE_LICENSE "ON")
set(CPACK_GENERATOR "NSIS")
set(CPACK_INSTALL_CMAKE_PROJECTS "F:/Invisible_Girl_Game;Invisible_Girl_Game;ALL;/")
set(CPACK_INSTALL_PREFIX "C:/Program Files (x86)/Invisible_Girl_Game")
set(CPACK_MODULE_PATH "")
set(CPACK_MONOLITHIC_INSTALL "ON")
set(CPACK_NSIS_CONTACT "team@sfml-dev.org")
set(CPACK_NSIS_DISPLAY_NAME "SFML 3.0.0 (MSVC 19.31.31107.0)")
set(CPACK_NSIS_DISPLAY_NAME_SET "TRUE")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "!define MUI_WELCOMEFINISHPAGE_BITMAP \"F:\\Invisible_Girl_Game\\External_Libraries\\SFML\\tools\\nsis\\sidebar.bmp\"
!define MUI_HEADERIMAGE_BITMAP \"F:\\Invisible_Girl_Game\\External_Libraries\\SFML\\tools\\nsis\\header.bmp\"
!define MUI_ICON \"F:\\Invisible_Girl_Game\\External_Libraries\\SFML\\tools\\nsis\\sfml.ico\"")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
set(CPACK_NSIS_PACKAGE_NAME "SFML 3.0.0 (MSVC 19.31.31107.0)")
set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")
set(CPACK_OUTPUT_CONFIG_FILE "F:/Invisible_Girl_Game/CPackConfig.cmake")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "F:/CMake/share/cmake-3.24/Templates/CPack.GenericDescription.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Invisible_Girl_Game built using CMake")
set(CPACK_PACKAGE_FILE_NAME "SFML-3.0.0-MSVC-19.31.31107.0-Release")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "SFML 3.0.0")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "SFML 3.0.0")
set(CPACK_PACKAGE_NAME "Invisible_Girl_Game")
set(CPACK_PACKAGE_NAME_SUMMARY "Simple and Fast Multimedia Library")
set(CPACK_PACKAGE_RELOCATABLE "true")
set(CPACK_PACKAGE_VENDOR "SFML Team")
set(CPACK_PACKAGE_VERSION "3.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "3")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_RESOURCE_FILE_LICENSE "F:/Invisible_Girl_Game/External_Libraries/SFML/license.md")
set(CPACK_RESOURCE_FILE_README "F:/Invisible_Girl_Game/External_Libraries/SFML/readme.md")
set(CPACK_RESOURCE_FILE_WELCOME "F:/CMake/share/cmake-3.24/Templates/CPack.GenericWelcome.txt")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_7Z "ON")
set(CPACK_SOURCE_GENERATOR "7Z;ZIP")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "F:/Invisible_Girl_Game/CPackSourceConfig.cmake")
set(CPACK_SOURCE_ZIP "ON")
set(CPACK_SYSTEM_NAME "win64")
set(CPACK_THREADS "1")
set(CPACK_TOPLEVEL_TAG "win64")
set(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "F:/Invisible_Girl_Game/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
