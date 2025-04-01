cmake_minimum_required(VERSION 3.12)
project(ZappyProject)

add_subdirectory(Server)
add_subdirectory(GUI)

add_custom_target(copy_zappy_server ALL
  COMMAND ${CMAKE_COMMAND} -E echo "Source file path: ${CMAKE_BINARY_DIR}/Server/zappy_server"
  COMMAND ${CMAKE_COMMAND} -E echo "Destination path: ${CMAKE_SOURCE_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/Server/zappy_server ${CMAKE_SOURCE_DIR}
  #DEPENDS Server
  DEPENDS zappy_server
  COMMENT "Copying zappy_server to project root"
)

add_custom_target(copy_zappy_gui ALL
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/GUI/zappy_gui ${CMAKE_SOURCE_DIR}
  DEPENDS zappy_gui
  COMMENT "Copying zappy_gui to project root"
)

add_custom_target(zappy_ai ALL
  COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/IA
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/IA/zappy_ai ${CMAKE_SOURCE_DIR}
  COMMENT "Building the Python AI component using Makefile"
)

add_custom_target(build_all
  DEPENDS copy_zappy_server copy_zappy_gui zappy_ai
  COMMENT "Building all components"
)

add_custom_target(fclean
  COMMAND make clean
  COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/zappy_server
  COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/zappy_gui
  COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/zappy_ai
  COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/IA fclean
  COMMENT "Cleaning all build files from project root, including zappy_ia in IA folder"
)

add_custom_target(re
  COMMAND ${CMAKE_COMMAND} --build . --target fclean
  COMMAND ${CMAKE_COMMAND} --build . --target build_all
  COMMENT "Rebuilding all components"
)
