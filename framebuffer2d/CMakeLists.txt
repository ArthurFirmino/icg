get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SHADERS "*.glsl")

add_executable(${EXERCISENAME} ${SOURCES} ${HEADERS} ${SHADERS})
if(WIN32)
        target_link_libraries(${EXERCISENAME} "legacy_stdio_definitions.lib")
endif()
target_link_libraries(${EXERCISENAME} ${COMMON_LIBS})

#--- data need to be copied to run folder
file(COPY ${PROJECT_SOURCE_DIR}/framebuffer2d/nyancat.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/framebuffer2d/background.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
