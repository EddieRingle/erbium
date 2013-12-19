project(erbium C)

cmake_minimum_required(VERSION 2.8.8)

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

include(GetGitRevisionDescription)
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
configure_file("${ER_SOURCE_DIR}/version.c.in" "${ER_SOURCE_DIR}/version.c")

aux_source_directory(. SOURCES)
list(APPEND SOURCES "${ER_SOURCE_DIR}/version.c" "${ER_INCLUDE_DIR}/erbium_version.h")

add_executable(${PROJECT_NAME} ${SOURCES})
