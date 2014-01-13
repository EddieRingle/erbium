find_package(OpenGL REQUIRED)
list(APPEND er_LIBS ${OPENGL_gl_LIBRARY})
if(APPLE)
	find_library(COCOA_FRAMEWORK Cocoa)
	find_library(IOKIT_FRAMEWORK IOKit)
	find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
	find_library(CORE_VIDEO_FRAMEWORK CoreVideo)
	list(APPEND er_LIBS
		${COCOA_FRAMEWORK}
		${IOKIT_FRAMEWORK}
		${CORE_FOUNDATION_FRAMEWORK}
		${CORE_VIDEO_FRAMEWORK})
endif()

if(APPLE)
	add_executable(main_test MACOSX_BUNDLE main_test.c)
	set_target_properties(main_test PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Main Test")
	add_executable(window_test MACOSX_BUNDLE window_test.c)
	set_target_properties(window_test PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Window Test")
	add_executable(entity_test MACOSX_BUNDLE entity_test.c)
	set_target_properties(entity_test PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Entity Test")
else()
	add_executable(main_test WIN32 main_test.c)
	add_executable(window_test WIN32 window_test.c)
	add_executable(entity_test WIN32 entity_test.c)
endif()

set(BINARIES main_test window_test entity_test)

if(MSVC)
    set_target_properties(${BINARIES} PROPERTIES
        LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

if(APPLE)
	set_target_properties(${BINARIES} PROPERTIES
		MACOSX_BUNDLE_SHORT_VERSION_STRING ${er_VERSION_FULL}
		MACOSX_BUNDLE_LONG_VERSION_STRING ${er_VERSION_FULL})
endif()

foreach(binary ${BINARIES})
    target_link_libraries(${binary} erbium_combined ${er_LIBS})
endforeach()
