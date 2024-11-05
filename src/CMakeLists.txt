file(
 GLOB_RECURSE final_project_INC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.h *.hpp
)

file(
 GLOB_RECURSE final_project_SRC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.c *.cpp
)
#Copies this final_project's asset folder to bin when it is built
add_custom_target(copyAssets ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/assets/
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${final_project_INC} DESTINATION include/final_project)
add_executable(final_project ${final_project_SRC} ${final_project_INC})
target_link_libraries(final_project PUBLIC core IMGUI glm)
target_include_directories(final_project PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR})

#Trigger asset copy when final_project is built
add_dependencies(final_project copyAssets)