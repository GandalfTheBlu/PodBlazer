#--------------------------------------------------------------------------
# engine
#--------------------------------------------------------------------------

SET(engine_files 
	window.h
	window.cc
	stb_image.h
	mesh.h
	mesh.cc
	texture.h
	texture.cc
	shader.h
	shader.cc
	renderer.h
	renderer.cc
	transform.h
	transform.cc
	camera.h
	camera.cc
	material.h
	file_reader.h
	file_reader.cc
)
SOURCE_GROUP("engine" FILES ${engine_files})
ADD_LIBRARY(engine STATIC ${engine_files})
ADD_DEPENDENCIES(engine glew glfw)
TARGET_INCLUDE_DIRECTORIES(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
TARGET_LINK_LIBRARIES(engine PUBLIC exts glew glfw ${OPENGL_LIBS})

