project(lua51 C)

cmake_minimum_required(VERSION 2.8.8)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

set(lualib_SRCS
	src/lapi.c src/lcode.c src/ldebug.c src/ldo.c src/ldump.c
	src/lfunc.c src/lgc.c src/llex.c src/lmem.c src/lobject.c
	src/lopcodes.c src/lparser.c src/lstate.c src/lstring.c
	src/ltable.c src/ltm.c src/lundump.c src/lvm.c src/lzio.c
	src/lauxlib.c src/lbaselib.c src/ldblib.c src/liolib.c
	src/lmathlib.c src/loslib.c src/ltablib.c src/lstrlib.c
	src/loadlib.c src/linit.c)

include_directories(src)

if(NOT MINGW AND NOT MSVC)
  add_definitions(-DLUA_USE_POSIX -DLUA_USE_DLOPEN)
endif()

add_library(lua51 STATIC ${lualib_SRCS})

install(TARGETS lua51 EXPORT lua51 ARCHIVE DESTINATION lib)
install(FILES src/lua.h src/luaconf.h src/lualib.h src/lauxlib.h
	DESTINATION include/lua5.1)
