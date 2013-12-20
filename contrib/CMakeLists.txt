if(NOT ER_CONTRIB_DOWNLOAD_DIR)
	set(ER_CONTRIB_DOWNLOAD_DIR ${ER_CONTRIB_DIR}/downloads)
endif()

include(ExternalFreetype)
include(ExternalGLEW)
include(ExternalGLFW)
include(ExternalLua51)
