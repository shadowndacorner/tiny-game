set(GAME_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PLATFORM_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platform)
set(GAME_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(GAME_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

file(
	GLOB_RECURSE GAME_SRCS 
	"${GAME_SRC_DIR}/*.cpp"
)

if (THIS_WILL_NOT_PASS)
    # tiny windows platform
    set(PLATFORM_SRC_DIR ${PLATFORM_ROOT_DIR}/windows)
    file(
        GLOB_RECURSE PLATFORM_SRCS 
        "${PLATFORM_ROOT_DIR}/windows/*.cpp"
        "${PLATFORM_ROOT_DIR}/windows/*.hpp"
        "${PLATFORM_ROOT_DIR}/windows/*.h"
        "${PLATFORM_ROOT_DIR}/opengl/*.c"
        "${PLATFORM_ROOT_DIR}/opengl/*.cpp"
        "${PLATFORM_ROOT_DIR}/opengl/*.hpp"
        "${PLATFORM_ROOT_DIR}/opengl/*.h"
    )
    set (PLATFORM_INCLUDES ${GLFW_INCLUDE_DIR})
    set (PLATFORM_LINKS GLFW)
elseif (WIN32 OR UNIX OR APPLE)
    # 
    set(PLATFORM_SRC_DIR ${PLATFORM_ROOT_DIR}/sdl)
    file(
        GLOB_RECURSE PLATFORM_SRCS 
        "${PLATFORM_ROOT_DIR}/sdl/*.cpp"
        "${PLATFORM_ROOT_DIR}/sdl/*.hpp"
        "${PLATFORM_ROOT_DIR}/sdl/*.h"
        "${PLATFORM_ROOT_DIR}/bgfx/*.c"
        "${PLATFORM_ROOT_DIR}/bgfx/*.cpp"
        "${PLATFORM_ROOT_DIR}/bgfx/*.hpp"
        "${PLATFORM_ROOT_DIR}/bgfx/*.h"
    )
    set (PLATFORM_INCLUDES ${BGFX_INCLUDE_DIR} ${SDL2_INCLUDE_DIR} ${BGFX_THIRD_PARTY_DIR})
    set (PLATFORM_LINKS SDL2 bgfx dear-imgui)
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
    ${STB_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${BOX2D_INCLUDE_DIR}
    ${CEREAL_INCLUDE_DIR}
    ${PLATFORM_INCLUDES}
)

target_link_libraries (
    tinygame
    box2d
    ${PLATFORM_LINKS}
)

set_target_properties(tinygame PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${GAME_DATA_DIR})

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