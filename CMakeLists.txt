project(erbium C)

cmake_minimum_required(VERSION 2.8.8)

#
# Options
#

option(ER_BUILD_EXAMPLES "Build Erbium examples" ON)

#
# Set up some variables
#

set(ER_ROOT_DIR "${CMAKE_SOURCE_DIR}")
set(ER_ASSETS_DIR "${ER_ROOT_DIR}/assets")
set(ER_CONTRIB_DIR "${ER_ROOT_DIR}/contrib")
set(ER_SCRIPTS_DIR "${ER_ROOT_DIR}/scripts")
set(ER_SOURCE_DIR "${ER_ROOT_DIR}/src")
set(ER_INCLUDE_DIR "${ER_ROOT_DIR}/include")
set(ER_OUT_DIR "${ER_ROOT_DIR}/out")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif()

list(APPEND CMAKE_MODULE_PATH "${ER_SCRIPTS_DIR}/cmake/modules")

#
# Modules/macros
#

include(GetGitRevisionDescription)
include(GroupByFolder)
include(MergeStaticLibs)
include(TargetArch)

#
# Target Arch/OS detection
#

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(ER_TARGET_OS "darwin")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(ER_TARGET_OS "linux")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(ER_TARGET_OS "windows")
else()
	set(ER_TARGET_OS "${CMAKE_SYSTEM_NAME}")
endif()

target_architecture(ER_TARGET_ARCH)

set(ER_TARGET_OUT_DIR "${ER_OUT_DIR}/${ER_TARGET_OS}_${ER_TARGET_ARCH}")
set(ER_TARGET_CONTRIB_OUT_DIR "${ER_TARGET_OUT_DIR}/contrib")

set(CMAKE_INSTALL_PREFIX ${ER_TARGET_OUT_DIR})

#
# Generate version information from git-describe
#

git_describe(gitdesc)

if("${gitdesc}" EQUAL "GIT-NOTFOUND")
	message(FATAL_ERROR "Erbium must be built within a live Git root")
	return()
endif()
set(er_VERSION_FULL "${gitdesc}")
if(er_VERSION_FULL MATCHES "([0-9]+)\\.([0-9]+)\\.([0-9]+)-?([0-9]+-g[0-9a-f]+)?")
	set(er_VERSION_MAJOR ${CMAKE_MATCH_1})
	set(er_VERSION_MINOR ${CMAKE_MATCH_2})
	set(er_VERSION_PATCH ${CMAKE_MATCH_3})
	set(er_VERSION_EXTRA ${CMAKE_MATCH_4})
endif()

message(STATUS "Building version: ${er_VERSION_FULL}")
configure_file("${ER_SOURCE_DIR}/er_version.c.in" "${CMAKE_CURRENT_BINARY_DIR}/er_version.c")

#
# Dependencies
#

find_package(Threads REQUIRED)

include("${ER_CONTRIB_DIR}/CMakeLists.txt")

#
# Erbium
#

set(erbium_SOURCES
	${ER_SOURCE_DIR}/app.c
	${ER_SOURCE_DIR}/context.c
	${ER_SOURCE_DIR}/context_glfw.c
	${ER_SOURCE_DIR}/entity.c
	${ER_SOURCE_DIR}/fs.c
	${ER_SOURCE_DIR}/graphics_glfw.c
	${ER_SOURCE_DIR}/internal.h
	${ER_SOURCE_DIR}/io.c
	${ER_SOURCE_DIR}/io_glfw.c
	${ER_SOURCE_DIR}/matrix.c
	${ER_SOURCE_DIR}/properties.c
	${ER_SOURCE_DIR}/scene.c
	${ER_SOURCE_DIR}/string_utils.c
	${ER_SOURCE_DIR}/subsystem.c
	${ER_SOURCE_DIR}/subsystem/subsystem_defaults.h
	${ER_SOURCE_DIR}/subsystem/subsystem_renderer.c
	${ER_SOURCE_DIR}/subsystem/renderer/renderer.h
	${ER_SOURCE_DIR}/subsystem/renderer/renderer_gl.c
	${ER_SOURCE_DIR}/uthash.h
	${ER_SOURCE_DIR}/ccan/build_assert.h
	${ER_SOURCE_DIR}/ccan/check_type.h
	${ER_SOURCE_DIR}/ccan/container_of.h
	${ER_SOURCE_DIR}/ccan/json.c
	${ER_SOURCE_DIR}/ccan/json.h
	${ER_SOURCE_DIR}/ccan/list.c
	${ER_SOURCE_DIR}/ccan/list.h
	${CMAKE_CURRENT_BINARY_DIR}/er_version.c
)

set(erbium_INCLUDES
	${ER_INCLUDE_DIR}/erbium.h
	${ER_INCLUDE_DIR}/erbium_version.h
	${ER_INCLUDE_DIR}/platform_detect.h
)

include_directories(${ER_INCLUDE_DIR})
include_directories(SYSTEM "${ER_TARGET_CONTRIB_OUT_DIR}/include")
include_directories(SYSTEM "${ER_TARGET_CONTRIB_OUT_DIR}/include/freetype2")
include_directories(SYSTEM "${ER_TARGET_OUT_DIR}/include")

link_directories("${ER_TARGET_CONTRIB_OUT_DIR}/lib")
link_directories("${ER_TARGET_OUT_DIR}/lib")

if(NOT MSVC)
    add_definitions(-std=c11 -Wall -pedantic)
    if(${CMAKE_BUILD_TYPE} MATCHES "Debug" OR ${CMAKE_BUILD_TYPE} MATCHES "RelWithDebInfo")
        add_definitions(-O0 -g)
    else()
        add_definitions(-Werror)
        add_definitions(-O2)
    endif()
endif()

add_library(erbium STATIC ${erbium_SOURCES} ${erbium_INCLUDES})
set(erbium_DEPENDENCIES
	freetype
	glew
	glfw
	libpng
	lua51
	ogg
	vorbis
	vorbisfile
	zlib
)

if(NOT APPLE)
	list(APPEND erbium_DEPENDENCIES openal)
endif()

add_dependencies(erbium ${erbium_DEPENDENCIES})
set_target_properties(erbium PROPERTIES
	OUTPUT_NAME erbium_nodeps
)

merge_static_libs(erbium_combined OUTPUT_NAME erbium
	erbium
	${erbium_DEPENDENCIES}
)

if(ER_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()

configure_file(${ER_ROOT_DIR}/ErbiumConfig.cmake.in
	${erbium_BINARY_DIR}/ErbiumConfig.cmake @ONLY)

install(FILES ${erbium_BINARY_DIR}/ErbiumConfig.cmake
	DESTINATION lib${LIB_SUFFIX}/cmake/erbium)

group_by_folder(${ER_ROOT_DIR} "${erbium_SOURCES}")
group_by_folder(${ER_ROOT_DIR} "${erbium_INCLUDES}")
