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

set(CPACK_PRODUCTBUILD_DOMAINS TRUE)
set(CPACK_PRODUCTBUILD_DOMAINS_ANYWHERE TRUE)
set(CPACK_PRODUCTBUILD_DOMAINS_USER TRUE)
set(CPACK_PRODUCTBUILD_DOMAINS_ROOT TRUE)

set(CPACK_PACKAGE_INSTALL_DIRECTORY /)
set(CPACK_PACKAGING_INSTALL_PREFIX /)

# Installer texts
set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/src/resources/macos-welcome.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/src/resources/macos-readme.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/src/resources/gpl-3.0.rtf")

# Package generator defaults. Override using "cpack -G [generator]"
set(CPACK_GENERATOR productbuild)
set(CPACK_SOURCE_GENERATOR TGZ)

include(CPack)
