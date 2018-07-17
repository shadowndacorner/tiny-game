set(BOX2D_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Box2D/Box2D)

file(
	GLOB_RECURSE BOX2D_SRCS 
    "${BOX2D_SRC_DIR}/*.cpp"
    "${BOX2D_SRC_DIR}/*.c"
)

file(
	GLOB_RECURSE BOX2D_INCLUDES
    "${BOX2D_INCLUDE_DIR}/*.h"
    "${BOX2D_INCLUDE_DIR}/*.hpp"
)

add_library (Box2D STATIC ${BOX2D_SRCS} ${BOX2D_INCLUDES})

target_include_directories (
    Box2D
    PUBLIC
    ${BOX2D_INCLUDE_DIR}
)

#target_link_libraries (
#    Box2D
#)

source_group(
	TREE "${BOX2D_SRC_DIR}"
	PREFIX "Source"
	FILES ${BOX2D_SRCS}
)

source_group(
	TREE "${BOX2D_INCLUDE_DIR}"
	PREFIX "Headers"
	FILES ${BOX2D_INCLUDES}
)