cmake_minimum_required(VERSION 3.17) 
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake")
include (prevent_source_builds)
include (clang_tidy)

add_executable(${PROJECT_NAME}
  main.cpp
  computer_player.cpp
  computer_player.h
  game.cpp
  game.h
  options.cpp
  options.h
)
