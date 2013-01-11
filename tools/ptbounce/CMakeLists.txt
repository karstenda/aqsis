project(ptbounce)

set(ptbounce_srcs
	ptbounce.cpp
)

if(AQSIS_ENABLE_MPDUMP)
	add_definitions(-DENABLE_MPDUMP)
endif()

ptbounce_add_executable(ptbounce ${ptbounce_srcs}
	LINK_LIBRARIES aqsis_core aqsis_util)

aqsis_install_targets(aqsis)
