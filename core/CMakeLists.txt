file(
 GLOB_RECURSE CORE_INC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.h *.hpp
)

file(
 GLOB_RECURSE CORE_SRC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.c *.cpp
)

set(CORE_INC_DIR
 ${CMAKE_CURRENT_SOURCE_DIR}
 CACHE PATH "CORE INCLUDE SOURCE PATH"
)


add_library(core STATIC ${CORE_SRC} ${CORE_INC} "idk/shader.h" "idk/shader.cpp" "idk/texture2d.h" "idk/texture2d.cpp" "idk/camera.h" "idk/camera.cpp" "ew/external/glad.h" "ew/external/glad.cpp")

find_package(OpenGL REQUIRED)
if(OPENGL_VERSION_MAJOR LESS 4)
    message(FATAL_ERROR "OpenGL version 4.x or higher is required.")
endif()
target_link_libraries(core PUBLIC IMGUI)
target_link_libraries(core PUBLIC IMGUI glm)

install (TARGETS core DESTINATION lib)
install (FILES ${CORE_INC} DESTINATION include/core)

