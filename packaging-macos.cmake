# projectMSDL Default Packaging Configuration for Linux

# General packaging variables
set(CPACK_PACKAGE_NAME "projectM")
set(CPACK_PACKAGE_VENDOR "The projectM Development Team")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/resources/package-description.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A standalone, Milkdrop-like audio visualization application")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://projectm-visualizer.org/")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/src/resources/gpl-3.0.txt")
set(CPACK_STRIP_FILES TRUE)

### Productbuild configuration
set(CPACK_PKGBUILD_IDENTITY_NAME "${CODESIGN_IDENTITY_INSTALLER}")
set(CPACK_PRODUCTBUILD_IDENTITY_NAME "${CODESIGN_IDENTITY_INSTALLER}")
set(CPACK_PRODUCTBUILD_IDENTIFIER "org.projectm-visualizer.projectmsdl")

string(REPLACE ";" "," INSTALL_ARCHITECTURES "${CMAKE_OSX_ARCHITECTURES}")
file(WRITE "${CMAKE_BINARY_DIR}/CPackAdditionalConfig.cmake" "set(CPACK_OSX_ARCHITECTURES \"${INSTALL_ARCHITECTURES}\")")
set(CPACK_PROJECT_CONFIG_FILE "${CMAKE_BINARY_DIR}/CPackAdditionalConfig.cmake")

# Installer texts
set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/src/resources/macos-welcome.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/src/resources/macos-readme.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/src/resources/gpl-3.0.rtf")

# Package generator defaults. Override using "cpack -G [generator]"
set(CPACK_GENERATOR productbuild)
set(CPACK_SOURCE_GENERATOR TGZ)

include(CPack)

cpack_add_component(projectMSDL
        PLIST "${CMAKE_SOURCE_DIR}/src/resources/projectMSDL-component.plist"
)
