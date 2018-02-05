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
file(COPY ${PROJECT_SOURCE_DIR}/mouse/_multiline/line_fshader.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/_multiline)
file(COPY ${PROJECT_SOURCE_DIR}/mouse/_multiline/line_vshader.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/_multiline)

file(COPY ${PROJECT_SOURCE_DIR}/mouse/_point/point_vshader.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/_point)
file(COPY ${PROJECT_SOURCE_DIR}/mouse/_point/point_fshader.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/_point)
file(COPY ${PROJECT_SOURCE_DIR}/mouse/_point/point_selection_vshader.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/_point)
file(COPY ${PROJECT_SOURCE_DIR}/mouse/_point/point_selection_fshader.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/_point)

file(COPY ${PROJECT_SOURCE_DIR}/mouse/_quad/quad_vshader.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/_quad)
file(COPY ${PROJECT_SOURCE_DIR}/mouse/_quad/quad_fshader.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/_quad)
file(COPY ${PROJECT_SOURCE_DIR}/mouse/_quad/quad_texture.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/_quad)
