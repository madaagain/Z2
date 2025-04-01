cmake_minimum_required(VERSION 3.12)
project(zappy_server LANGUAGES C)

# @brief: Défini le chemin des sources et des includes
set(SRC_PATH "src/")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/includes")

# @brief: Liste des fichiers source
set(SOURCES
    ${SRC_PATH}dash_help.c
    ${SRC_PATH}parse_arg.c
    ${SRC_PATH}launch_loop.c
    ${SRC_PATH}in_the_loop.c
    ${SRC_PATH}print_info.c
    ${SRC_PATH}handle_struct_arg.c
    ${SRC_PATH}main.c
    ${SRC_PATH}resources.c
    ${SRC_PATH}movement.c
    ${SRC_PATH}ia_command.c
    ${SRC_PATH}game_info.c
    ${SRC_PATH}command.c
    ${SRC_PATH}look.c
    ${SRC_PATH}incantation.c
    ${SRC_PATH}free_struct.c
    ${SRC_PATH}graphic_command.c
    ${SRC_PATH}tile_content.c
    ${SRC_PATH}queue.c
    ${SRC_PATH}broadcast.c
    ${SRC_PATH}map.c
    ${SRC_PATH}send_graphic.c
    ${SRC_PATH}egg.c
    ${SRC_PATH}verif_incantation.c
)

# @brief: Défini les flags du compilateur
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g3")

# @brief: Inclure les répertoires
include_directories(${INCLUDE_DIR})

# @brief: Ajout de l'exécutable
add_executable(${PROJECT_NAME} ${SOURCES})

# @brief: Lien avec la bibliothèque mathématique
target_link_libraries(${PROJECT_NAME} m)

# Configuration des bibliothèques et des liens
# Lier les bibliothèques si nécessaire
# target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

# @brief: Tests Unitaires (optionnel, dépend des fichiers et bibliothèques de tests spécifiques)
# enable_testing()
# find_package(Criterion REQUIRED)
# set(TEST_SOURCES
#     @brief: Spécifie les fichiers source de test ici @Max, @Pablo, @Alexis
# )
# add_executable(unit_tests ${TEST_SOURCES})
# target_link_libraries(unit_tests Criterion::Criterion)
# add_test(NAME unit_tests COMMAND unit_tests)
