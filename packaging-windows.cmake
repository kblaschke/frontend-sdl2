# projectMSDL Default Packaging Configuration for Linux

# General packaging variables
set(CPACK_PACKAGE_NAME "projectMSDL")
set(CPACK_PACKAGE_VENDOR "The projectM Development Team")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/resources/package-description.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A standalone, Milkdrop-like audio visualization application")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://projectm-visualizer.org/")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/src/resources/gpl-3.0.txt")
set(CPACK_STRIP_FILES TRUE)

# WiX generator - requires WiX Toolset 3

# Start menu entry
set_property(INSTALL $<TARGET_FILE_NAME:projectMSDL>
        PROPERTY CPACK_START_MENU_SHORTCUTS "projectMSDL"
        )

set(CPACK_WIX_UPGRADE_GUID "7d15f9b9-1122-4d5e-bc31-61bb93d7480a")
set(CPACK_WIX_LICENSE_RTF "${CMAKE_SOURCE_DIR}/src/resources/gpl-3.0.rtf")
set(CPACK_WIX_PRODUCT_ICON "${CMAKE_SOURCE_DIR}/src/resources/icons/icon.ico")
set(CPACK_WIX_PROGRAM_MENU_FOLDER "projectM")
set(CPACK_WIX_PROPERTY_ARPHELPLINK "https://github.com/projectM-visualizer/projectm/wiki")

# Package generator defaults. Override using "cpack -G [generator]"
set(CPACK_GENERATOR ZIP)
set(CPACK_SOURCE_GENERATOR ZIP)

include(CPack)
