set(GAME_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PLATFORM_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platform)
set(GAME_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(GAME_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

file(
	GLOB_RECURSE GAME_SRCS 
	"${GAME_SRC_DIR}/*.cpp"
)

if (WIN32)
    set(PLATFORM_SRC_DIR ${PLATFORM_ROOT_DIR}/windows)
    file(
        GLOB_RECURSE PLATFORM_SRCS 
        "${PLATFORM_ROOT_DIR}/windows/*.cpp"
        "${PLATFORM_ROOT_DIR}/windows/*.hpp"
        "${PLATFORM_ROOT_DIR}/windows/*.h"
        "${PLATFORM_ROOT_DIR}/opengl/*.c"
        "${PLATFORM_ROOT_DIR}/opengl/*.cpp"
        "${PLATFORM_ROOT_DIR}/opengl/*.hpp"
    )
else()
    message(FATAL_ERROR "Attempting to run cmake for unsupported platform" )
endif()

file(
	GLOB_RECURSE GAME_INCLUDES
	"${GAME_INCLUDE_DIR}/*.hpp"
)

add_executable (tinygame ${GAME_SRCS} ${PLATFORM_SRCS} ${GAME_INCLUDES})

target_include_directories (
    tinygame
    PUBLIC
    ${GAME_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${STB_INCLUDE_DIR}
)

target_link_libraries (
	tinygame
    ${PLATFORM_LINKS}
)

source_group(
	TREE "${GAME_INCLUDE_DIR}" 
	PREFIX "Headers"
	FILES ${GAME_INCLUDES}
)

source_group(
	TREE "${GAME_SRC_DIR}" 
	PREFIX "Source"
	FILES ${GAME_SRCS}
)

source_group(
	TREE "${PLATFORM_ROOT_DIR}" 
	PREFIX "Platform Source"
	FILES ${PLATFORM_SRCS}
)