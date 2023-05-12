#--------------------------------------------------------------------------
# game
#--------------------------------------------------------------------------

PROJECT(game)

SET(game_files 
	main.cc
	game_app.h
	game_app.cc
	game_object.h
	game_object.cc
	world_settings.h
	world_settings.cc
	player.cc
	player.h
	map_generator.cc
	map_generator.h
	materials.h
	materials.cc
	point_system.cc
	point_system.h
)
SOURCE_GROUP("code" FILES ${game_files})

ADD_EXECUTABLE(game ${game_files})
TARGET_LINK_LIBRARIES(game engine)
ADD_DEPENDENCIES(game engine)

IF(MSVC)
	SET_PROPERTY(TARGET game PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
ENDIF(MSVC)