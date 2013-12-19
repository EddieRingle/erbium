include(ExternalProject)

set(lua51_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/lua51)

set(lua51_CMAKE_ARGS
	-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
	-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
	-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
	-DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
)

ExternalProject_Add(lua51_build
	PREFIX ${lua51_PREFIX}

	DOWNLOAD_DIR ${ER_CONTRIB_DOWNLOAD_DIR}

	URL http://www.lua.org/ftp/lua-5.1.5.tar.gz
	URL_MD5 2e115fe26e435e33b0d5c022e4490567

	PATCH_COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ER_SCRIPTS_DIR}/cmake/modules/lua51.cmake <SOURCE_DIR>/CMakeLists.txt

	INSTALL_DIR ${ER_TARGET_CONTRIB_OUT_DIR}
	CMAKE_ARGS ${lua51_CMAKE_ARGS}
)

add_library(lua51 STATIC IMPORTED)
set_target_properties(lua51 PROPERTIES
	IMPORTED_LOCATION ${ER_TARGET_CONTRIB_OUT_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}lua51${CMAKE_STATIC_LIBRARY_SUFFIX}
)
add_dependencies(lua51 lua51_build)
