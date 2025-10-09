# Note: each DESTINATION keyword needs its own COMPONENT specification!
install(TARGETS projectMSDL
        RUNTIME_DEPENDENCY_SET projectMSDLDepends
        RUNTIME DESTINATION ${PROJECTMSDL_BIN_DIR}
        COMPONENT projectMSDL
        BUNDLE DESTINATION . # .app bundle will reside at the top-level of the install prefix
        COMPONENT projectMSDL
        )

install(FILES ${PROJECTM_CONFIGURATION_FILE}
        DESTINATION ${PROJECTMSDL_DATA_DIR}
        RENAME ${PROJECTMSDL_PROPERTIES_FILENAME}
        COMPONENT projectMSDL
        )

if(ENABLE_INSTALL_BDEPS)
    install(RUNTIME_DEPENDENCY_SET projectMSDLDepends
            COMPONENT projectMSDL

            # Important: Due to CMake bug #24606 this needs to stay at the top of the argument list!
            # Exclude OS libraries on Linux/macOS
            POST_EXCLUDE_REGEXES
            "^/lib(32|64)?/+"
            "^/usr/lib(32|64)?/+"
            "^/Library/+"
            ".*system32/.*\\.dll"
            PRE_EXCLUDE_REGEXES
            ".*api-ms-win-crt-.*\\.dll"

            LIBRARY DESTINATION ${PROJECTMSDL_LIB_DIR}
            RUNTIME DESTINATION ${PROJECTMSDL_LIB_DIR}
            FRAMEWORK DESTINATION ${PROJECTMSDL_DATA_DIR}
            )
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND NOT ENABLE_FLAT_PACKAGE)
    if(ENABLE_DESKTOP_ICON)
        install(FILES src/resources/projectMSDL.desktop
                DESTINATION ${PROJECTMSDL_DESKTOP_DIR}
                COMPONENT projectMSDL
                )

        macro(INSTALL_ICON size)
            install(FILES src/resources/icons/icon_${size}x${size}.png
                    DESTINATION ${PROJECTMSDL_ICONS_DIR}/${size}x${size}/apps
                    RENAME projectMSDL.png
                    COMPONENT projectMSDL
                    )
        endmacro()

        foreach(size 16 32 48 64 72 96 128 256)
            install_icon(${size})
        endforeach()

        install(FILES src/resources/icons/icon_scalable.svg
                DESTINATION ${PROJECTMSDL_ICONS_DIR}/scalable/apps
                RENAME projectMSDL.svg
                COMPONENT projectMSDL
                )

    endif()

elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND NOT ENABLE_FLAT_PACKAGE)
    install(FILES src/resources/icons/icon.icns
            DESTINATION ${PROJECTMSDL_DATA_DIR}
            RENAME projectMSDL.icns
            COMPONENT projectMSDL
            )
    install(FILES src/resources/gpl-3.0.txt
            DESTINATION ${PROJECTMSDL_DATA_DIR}
            COMPONENT projectMSDL
            )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        install(IMPORTED_RUNTIME_ARTIFACTS projectMSDL
                LIBRARY DESTINATION ${PROJECTMSDL_BIN_DIR}
                RUNTIME DESTINATION ${PROJECTMSDL_BIN_DIR}
                COMPONENT projectMSDL
                )
endif()

# Install optional presets
foreach(preset_dir ${PRESET_DIRS})
    install(DIRECTORY ${preset_dir}
            DESTINATION "${PROJECTMSDL_PRESETS_DIR}"
            COMPONENT projectMSDL
            PATTERN *.md EXCLUDE
            REGEX "(^|/)[.][^.]*" EXCLUDE
            )
endforeach()

# Install optional textures
foreach(texture_dir ${TEXTURE_DIRS})
    install(DIRECTORY ${texture_dir}
            DESTINATION "${PROJECTMSDL_TEXTURES_DIR}"
            COMPONENT projectMSDL
            PATTERN *.md EXCLUDE
            REGEX "(^|/)[.][^.]*" EXCLUDE
            )
endforeach()
