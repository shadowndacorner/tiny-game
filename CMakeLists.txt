cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 17)

project (tinygame VERSION 0.1 LANGUAGES CXX)
set(CMAKE_C_FLAGS_DEBUG "-DDEBUG")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (WIN32)
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

	add_definitions(-DTINYGAME_WIN32)
	set (PLATFORM_LINKS )
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