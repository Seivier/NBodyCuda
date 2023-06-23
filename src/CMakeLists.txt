
add_executable(NBodyCuda main.cpp shader.cpp shader.h buffer.cpp buffer.h)
target_link_libraries(NBodyCuda glfw glad ${OPENGL_LIBRARIES} imgui)

file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/shaders)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})