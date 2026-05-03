cmake_minimum_required(VERSION 3.16)


# Set root directory, if needed
if (NOT GFXF_ROOT_DIR)
    set(GFXF_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
endif()


# Set the name of the project
set(target_name GFXFramework)
project(${target_name} C CXX)


# Use C99 and C++17
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Include helper scripts
include(${GFXF_ROOT_DIR}/infra/utils.cmake)
custom_set_build_type()

# Fetch ImGui
include(FetchContent)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.91.5
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(imgui)

set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# Fix to hide a bug in CMake. Hopefully able to be removed in the future.
set(CMAKE_POLICY_DEFAULT_CMP0012 NEW)


# Find required packages
find_package(OpenGL REQUIRED)
if (NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    find_package(GLEW REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    find_package(spdlog REQUIRED)
    find_package(Freetype REQUIRED)
endif()

# Pin assimp to the same commit as the Windows prebuilt (525213a0, ~v5.0/5.1).
# The animation key structs (aiVectorKey, aiQuatKey) gained a new mInterpolation
# field in 6.0, which silently breaks our skeletal animation parsing if headers
# and library disagree on struct size. On macOS we build from source at the
# pinned commit; on Linux we accept the system package as long as it is < 6.
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    include(FetchContent)
    set(ASSIMP_BUILD_TESTS         OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ASSIMP_TOOLS  OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_SAMPLES       OFF CACHE BOOL "" FORCE)
    set(ASSIMP_INSTALL             OFF CACHE BOOL "" FORCE)
    set(ASSIMP_WARNINGS_AS_ERRORS  OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS          ON  CACHE BOOL "" FORCE)
    FetchContent_Declare(assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG        525213a0456c4be3c61923d5462976916df6d06a
    )
    FetchContent_MakeAvailable(assimp)
    # assimp's CMake hard-codes -Werror on the assimp target (not gated by
    # ASSIMP_WARNINGS_AS_ERRORS at this commit). Newer Apple Clang flags valid
    # warnings in this 4-year-old source that upstream tried to silence with
    # pragmas that no longer take effect. Drop -Werror on the dependency only;
    # warnings are still printed, just non-fatal.
    target_compile_options(assimp PRIVATE -Wno-error)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(assimp REQUIRED)
    if (DEFINED assimp_VERSION AND assimp_VERSION VERSION_GREATER_EQUAL 6.0)
        message(FATAL_ERROR
            "Found assimp ${assimp_VERSION}; this project pins to the 5.x ABI "
            "(aiVectorKey/aiQuatKey layout changed in 6.0). Install assimp 5.x.")
    endif()
endif()

# Set options
option(AI_NPC_GAME "Build AI NPC game source files" ON)
option(WITH_LAB_M1 "With module 1 labs" OFF)
option(WITH_LAB_M2 "With module 2 labs" OFF)
option(WITH_LAB_EXTRA "With extra labs" OFF)
option(USE_DEV_COMPONENTS "Use dev components" OFF)


# Set RPATH to avoid using LD_LIBRARY_PATH
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CMAKE_INSTALL_RPATH "@executable_path")
else()
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
endif()


# Set the suffixes for directories, depending on how we want to include
# sources and headers for different courses. If the suffix variables remain
# empty, the GLOB_RECURSE will look for files in the directory called `lab_`
# which does not actually exist, but will not fail.
if (AI_NPC_GAME)
    set(AI_NPC_GAME_DIR "ai_npc_game")
endif()
if (WITH_LAB_M1)
    set(SUFFIX_LAB_M1 "m1")
endif()
if (WITH_LAB_M2)
    set(SUFFIX_LAB_M2 "m2")
endif()
if (WITH_LAB_EXTRA)
    set(SUFFIX_LAB_EXTRA "extra")
endif()


# Gather the source files
file(GLOB_RECURSE GFXF_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/components/*.c*
    ${CMAKE_CURRENT_LIST_DIR}/src/core/*.c*
    ${CMAKE_CURRENT_LIST_DIR}/src/utils/*.c*
    #
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    #
    ${CMAKE_CURRENT_LIST_DIR}/src/${AI_NPC_GAME_DIR}/*.c*
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_M1}/*.c*
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_M2}/*.c*
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_EXTRA}/*.c*
)


# Gather the header files
file(GLOB_RECURSE GFXF_HEADERS_PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/components/*.h*
    ${CMAKE_CURRENT_LIST_DIR}/src/core/*.h*
    ${CMAKE_CURRENT_LIST_DIR}/src/utils/*.h*
    #
    ${CMAKE_CURRENT_LIST_DIR}/res/*.glsl
    #
    ${CMAKE_CURRENT_LIST_DIR}/src/${AI_NPC_GAME_DIR}/*.h*
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_M1}/*.h*
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_M2}/*.h*
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_EXTRA}/*.h*
    #
    ${CMAKE_CURRENT_LIST_DIR}/src/${AI_NPC_GAME_DIR}/*.glsl
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_M1}/*.glsl
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_M2}/*.glsl
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_EXTRA}/*.glsl
)


# Gather the include directories
set(GFXF_INCLUDE_DIRS_PRIVATE
    ${GFXF_ROOT_DIR}/deps/api
    ${CMAKE_CURRENT_LIST_DIR}/src
)


# Add the executable
custom_add_executable(${target_name}
    ${GFXF_SOURCES}
    ${GFXF_SOURCES_HIDDEN}
    ${GFXF_HEADERS_PRIVATE}
    ${IMGUI_SOURCES}
)

target_include_directories(${target_name} PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

# Detect architecture
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm|aarch")
        set(__cmake_arch arm64)
    else()
        set(__cmake_arch x86_64)
    endif()
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm|aarch")
        set(__cmake_arch armv7)
    else()
        set(__cmake_arch i686)
    endif()
endif()


# Set library suffic
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(__cmake_shared_suffix dll)
    set(__cmake_import_suffix lib)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(__cmake_shared_suffix so)
    set(__cmake_import_suffix so)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(__cmake_shared_suffix dylib)
    set(__cmake_import_suffix dylib)
endif()


# Link third-party libraries
target_link_libraries(${target_name} PRIVATE
    ${OPENGL_LIBRARIES}
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(${target_name} PRIVATE
        ${GFXF_ROOT_DIR}/deps/prebuilt/GL/${__cmake_arch}/glew32.lib
        ${GFXF_ROOT_DIR}/deps/prebuilt/GLFW/${__cmake_arch}/glfw3dll.lib
        ${GFXF_ROOT_DIR}/deps/prebuilt/assimp/${__cmake_arch}/assimp.lib
        ${GFXF_ROOT_DIR}/deps/prebuilt/spdlog/${__cmake_arch}/spdlog.lib
        ${GFXF_ROOT_DIR}/deps/prebuilt/freetype/${__cmake_arch}/freetype.lib
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(${target_name} PRIVATE
        GLEW
        glfw
        assimp
        spdlog
        freetype
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # Fix linking on 10.14+. See https://stackoverflow.com/questions/54068035
    target_link_directories(${target_name} PRIVATE
        /usr/local/lib
    )

    if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm|aarch")
        target_link_directories(${target_name} PRIVATE
        /opt/homebrew/lib
    )
    endif ()

    target_link_libraries(${target_name} PRIVATE
        GLEW
        glfw
        assimp
        spdlog
        freetype
    )
endif()

if (USE_DEV_COMPONENTS)
    if (NOT TARGET GFXComponents)
        add_subdirectory(src/components_hidden)
        target_link_libraries(${target_name} PRIVATE GFXComponents)
    endif()
else()
    target_link_libraries(${target_name} PRIVATE
        ${GFXF_ROOT_DIR}/deps/prebuilt/GFXComponents/${__cmake_arch}/GFXComponents.${__cmake_import_suffix}
    )
endif()


# Set target properties
target_include_directories(${target_name} PRIVATE ${GFXF_INCLUDE_DIRS_PRIVATE})


# For Visual Studio, set the working directory and the startup project
if (MSVC)
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT ${target_name})
    set_property(TARGET ${target_name} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()


# Set source file properties
set_source_files_properties(${GFXF_HEADERS_PRIVATE}  PROPERTIES HEADER_FILE_ONLY TRUE)


# Group files under logical folders (mainly for IDEs)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES
    ${GFXF_SOURCES}
    ${GFXF_SOURCES_HIDDEN}
    ${GFXF_HEADERS_PRIVATE}
)


# Add definitions (specific to this project)
# Recommended reading: so@a/24470998/5922876, so@a/11437693/5922876
set(GFXF_CXX_DEFS       LIBGFXC_EXPORTS GLM_FORCE_SILENT_WARNINGS _CRT_SECURE_NO_WARNINGS SOLVED $<$<CONFIG:Debug>:DEBUG>)
if (AI_NPC_GAME)
    set(GFXF_CXX_DEFS   ${GFXF_CXX_DEFS} AI_NPC_GAME)
endif()
if (WITH_LAB_M1)
    set(GFXF_CXX_DEFS   ${GFXF_CXX_DEFS} WITH_LAB_M1)
endif()
if (WITH_LAB_M2)
    set(GFXF_CXX_DEFS   ${GFXF_CXX_DEFS} WITH_LAB_M2)
endif()
if (WITH_LAB_EXTRA)
    set(GFXF_CXX_DEFS   ${GFXF_CXX_DEFS} WITH_LAB_EXTRA)
endif()
target_compile_definitions(${target_name} PRIVATE ${GFXF_CXX_DEFS})


# Add compile options (specific to this project)
# Recommended reading: so@a/23995391/5922876
if (MSVC)
    set(GFXF_CXX_FLAGS  /W4 /WX-)
    # TODO(developer): Maybe address these warnings, see discussion below.
    set(GFXF_CXX_FLAGS  ${GFXF_CXX_FLAGS} /wd4100 /wd4458 /wd4189)
else()
    set(GFXF_CXX_FLAGS  -Wall -Wextra -pedantic -Wno-error)
    # TODO(developer): Addressing these warnings might make the source code
    # more convoluted, since they are generated by missing code that needs
    # to be implemented. Some of the warnings are caused by dependencies.
    if (CMAKE_C_COMPILER_ID MATCHES "GNU")
        set(GFXF_CXX_FLAGS  ${GFXF_CXX_FLAGS}   -Wno-unused-parameter -Wno-unused-variable
                                                -Wno-unused-but-set-variable
                                                -Wno-missing-field-initializers -Wno-sign-compare)
    elseif (CMAKE_C_COMPILER_ID MATCHES "Clang")
        set(GFXF_CXX_FLAGS  ${GFXF_CXX_FLAGS}   -Wno-unused-parameter -Wno-unused-variable
                                                -Wno-missing-field-initializers -Wno-sign-compare
                                                # Apple clang
                                                -Wno-unknown-warning-option
                                                # Windows clang
                                                -Wno-microsoft-enum-value -Wno-language-extension-token)
    endif()
endif()
target_compile_options(${target_name} PRIVATE ${GFXF_CXX_FLAGS})


# Post-build events. First, we get the directory where the target was
# just built. We will then copy several files and create several symlinks
# into the target's parent directory.
get_target_property(__target_dir ${target_name} RUNTIME_OUTPUT_DIRECTORY)

# On Windows, we copy required binary files next to the target.
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${GFXF_ROOT_DIR}/deps/prebuilt/GL/${__cmake_arch}/glew32.dll"
            "${GFXF_ROOT_DIR}/deps/prebuilt/GLFW/${__cmake_arch}/glfw3.dll"
            "${GFXF_ROOT_DIR}/deps/prebuilt/assimp/${__cmake_arch}/assimp.dll"
            "${GFXF_ROOT_DIR}/deps/prebuilt/spdlog/${__cmake_arch}/spdlog.dll"
            "${GFXF_ROOT_DIR}/deps/prebuilt/freetype/${__cmake_arch}/freetype.dll"

            # Destination directory
            "${__target_dir}"
    )
endif()

# This bit is necessary to allow building via Clang on Windows
if (NOT MSVC)
    set(__error_sink || rem)
endif()

# We create symbolic links of several directories next to the target.
# Note for developers: a much more elegant solution would be to use
# `cmake -E rm` and `cmake -E create_symlink` and avoid using OS-specific
# commands. Unfortunately, `create_symlink` will fail if the user is not
# an administrator of the device, as it uses `mklink /D` under the hood.
foreach (dir IN ITEMS "src" "assets")
    if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        string(REPLACE "/" "\\" __target_dir "${__target_dir}")
        string(REPLACE "/" "\\" GFXF_ROOT_DIR "${GFXF_ROOT_DIR}")

        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND rmdir /S /Q "${__target_dir}\\${dir}" ${__error_sink}
            COMMAND mklink /J "${__target_dir}\\${dir}" "${GFXF_ROOT_DIR}\\${dir}" ${__error_sink}
        )
    else()
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND rm -rf "${__target_dir}/${dir}"
            COMMAND ln -s "${GFXF_ROOT_DIR}/${dir}" "${__target_dir}/${dir}"
        )
    endif()
endforeach()

# For builds that are supplied with closed-source modules,
# we copy those modules next to the target. If you already
# have access to the original source code, there's no need
# for this step.
if (NOT USE_DEV_COMPONENTS)
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GFXF_ROOT_DIR}/deps/prebuilt/GFXComponents/${__cmake_arch}/GFXComponents.${__cmake_shared_suffix}" "${__target_dir}"
    )
endif()

# On macOS the assimp dylib is produced by FetchContent in the build tree.
# CMAKE_INSTALL_RPATH is @executable_path, so copy the dylib next to the
# target after every build to make it resolvable at run time. Use
# TARGET_SONAME_FILE so the copied file matches the install_name the
# loader actually queries (e.g. libassimpd.5.dylib), not the fully
# versioned path TARGET_FILE points at (e.g. libassimpd.5.4.0.dylib).
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_SONAME_FILE:assimp>" "${__target_dir}"
    )
endif()
