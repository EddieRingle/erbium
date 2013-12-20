project(erbium C)

cmake_minimum_required(VERSION 2.8.8)

#
# Set up some variables
#

set(ER_ROOT_DIR "${CMAKE_SOURCE_DIR}")
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
configure_file("${ER_SOURCE_DIR}/version.c.in" "${CMAKE_CURRENT_BINARY_DIR}/version.c")

#
# Dependencies
#

include("${ER_CONTRIB_DIR}/CMakeLists.txt")

#
# Erbium
#

set(erbium_SOURCES
	${CMAKE_CURRENT_BINARY_DIR}/version.c
)

set(erbium_INCLUDES
	${ER_INCLUDE_DIR}/erbium.h
	${ER_INCLUDE_DIR}/erbium_version.h
	${ER_INCLUDE_DIR}/platform_detect.h
)

include_directories(${ER_INCLUDE_DIR})
include_directories(SYSTEM "${ER_TARGET_CONTRIB_OUT_DIR}/include")
include_directories(SYSTEM "${ER_TARGET_OUT_DIR}/include")

link_directories("${ER_TARGET_CONTRIB_OUT_DIR}/lib")
link_directories("${ER_TARGET_OUT_DIR}/lib")

add_library(erbium STATIC ${erbium_SOURCES} ${erbium_INCLUDES})
add_dependencies(erbium
	lua51
)
set_target_properties(erbium PROPERTIES
	OUTPUT_NAME erbium_nodeps
)

merge_static_libs(erbium_combined OUTPUT_NAME erbium
	erbium
	lua51
)

group_by_folder(${ER_ROOT_DIR} "${erbium_SOURCES}")
group_by_folder(${ER_ROOT_DIR} "${erbium_INCLUDES}")
