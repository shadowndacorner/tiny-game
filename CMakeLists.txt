cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 17)

project (tinygame VERSION 0.1 LANGUAGES CXX)
# set(CMAKE_C_FLAGS_DEBUG "-DDEBUG")

set ( TINYGAME_ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
set ( GAME_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gamedata )
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 3rdParty
    set( THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty )
    
    # GLFW
        set (GLFW_ROOT ${THIRDPARTY_DIR}/glfw)
        set (GLFW_STATIC true)
        set (GLFW_VULKAN_STATIC off)
        set (GLFW_INCLUDE_DIR ${THIRDPARTY_DIR}/glfw/include)

        add_subdirectory(${GLFW_ROOT})
    # End GLFW

    # SDL2
        set (SDL2_ROOT ${THIRDPARTY_DIR}/SDL-mirror)
        set (SDL2_INCLUDE_DIR ${SDL2_ROOT}/include)
        
        add_subdirectory(${SDL2_ROOT})
    # End SDL2
    
    # STB
        set (STB_INCLUDE_DIR ${THIRDPARTY_DIR}/stb)
    # End STB

    # GLM
        set (GLM_INCLUDE_DIR ${THIRDPARTY_DIR}/glm)
    # End GLM

    # Box2D
        set (BOX2D_ROOT_DIR ${THIRDPARTY_DIR}/box2d_cmake)
        set (BOX2D_INCLUDE_DIR ${BOX2D_ROOT_DIR}/Box2D)
        set (BUILD_DEMOS OFF)
        set (INSTALL_DEMOS OFF)
        set (BUILD_SHARED OFF)
        set (BUILD_STATIC ON)
        add_subdirectory(${BOX2D_ROOT_DIR})
    # End Box2D

    # BGFX
        set (BGFX_ROOT_DIR ${THIRDPARTY_DIR}/bgfx.cmake)
        set (BGFX_INCLUDE_DIR ${BGFX_ROOT_DIR}/bgfx/include)
        set (BGFX_THIRD_PARTY_DIR ${BGFX_ROOT_DIR}/bgfx/3rdparty)
        set (BX_INCLUDE_DIR ${BGFX_ROOT_DIR}/bx/include)
        set (BGFX_BUILD_EXAMPLES OFF)
        set (BGFX_INSTALL OFF)
        set (BGFX_INSTALL_EXAMPLES OFF)
        add_subdirectory(${BGFX_ROOT_DIR})
    # End BGFX

    set( CEREAL_INCLUDE_DIR ${THIRDPARTY_DIR}/cereal/include )# End 3rdParty

if (WIN32)
    if (DISABLE_EXCEPTIONS)
        # Disable exceptions and RTTI
        if(CMAKE_CXX_FLAGS MATCHES "/EHsc ")
            string(REPLACE "/EHsc" "/EHs-c-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
            message(STATUS "CMAKE_CXX_FLAGS matches /EHsc before end of string -- replaced...")
            message(STATUS "")
        endif()

        if(CMAKE_CXX_FLAGS MATCHES "/EHsc$")
            string(REPLACE "/EHsc" "/EHs-c-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
            message(STATUS "CMAKE_CXX_FLAGS matches /EHsc at end of string -- replaced...")
            message(STATUS "")
        endif()

        if(CMAKE_CXX_FLAGS MATCHES "/GR ")
            string(REPLACE "/GR" "/GR-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
            message(STATUS "CMAKE_CXX_FLAGS matches /GR before end of string -- replaced...")
            message(STATUS "")
        endif()

        if(CMAKE_CXX_FLAGS MATCHES "/GR$")
            string(REPLACE "/GR" "/GR-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
            message(STATUS "CMAKE_CXX_FLAGS matches /GR at end of string -- replaced...")
            message(STATUS "")
        endif()
    endif()

	add_definitions(-DTINYGAME_WIN32)
	set (PLATFORM_LINKS glfw)
ELSE ()
    # Not implemented yet
	add_definitions(-DTINYGAME_LINUX)
	set (PLATFORM_LINKS )
ENDIF()

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(BINARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIRECTORY}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIRECTORY}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIRECTORY})

#include the subdirectory containing our libs
add_subdirectory (game)

if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT tinygame)
endif()