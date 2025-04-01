cmake_minimum_required(VERSION 3.12)
project(zappy_gui LANGUAGES CXX)

# Options de compilation
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Définition des fichiers source commun
set(SOURCE_FILES src/main.cpp
                src/Rendering/Mouse.cpp
                src/Item/AItems.cpp
                src/Item/AStones.cpp
                src/Item/Deraumere.cpp
                src/Item/Food.cpp
                src/Item/Linemate.cpp
                src/Item/Mendiane.cpp
                src/Item/Phiras.cpp
                src/Item/Sibur.cpp
                src/Item/Thysmate.cpp
                src/Item/Egg.cpp
                src/Rendering/Window.cpp
                src/Rendering/Menu.cpp
                src/Rendering/Map.cpp
                src/Client/Client.cpp
                src/Client/ParseArg.cpp
                src/Parser/ParserServer.cpp
                src/GameData/GameData.cpp
                src/Rendering/Player.cpp
)

include_directories("includes/")
include_directories("includes/Factory")
include_directories("includes/GameData")
include_directories("includes/Interface")
include_directories("includes/Item")
include_directories("includes/Map")
include_directories("includes/Parser")
include_directories("includes/Players")
include_directories("includes/Rendering")
include_directories("includes/Client")
include_directories(/usr/local/include)

# Définition de l'exécutable
add_executable(zappy_gui ${SOURCE_FILES})

# Compiler flags
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(zappy_gui PRIVATE -Wall -Wextra -Werror -g3 -fno-gnu-unique)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(zappy_gui PRIVATE -Wall -Wextra -Werror -g3)
endif()

# Configuration spécifique pour chaque système d'exploitation
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # Include directories
    target_include_directories(zappy_gui PRIVATE
        ${CMAKE_SOURCE_DIR}/includes  # Mise à jour du chemin vers 'includes'
        /Users/yumikooo/SFML-2.6.1-macOS-clang-arm64/include)

    # Linker settings and libraries
    target_link_directories(zappy_gui PRIVATE
        /Users/yumikooo/SFML-2.6.1-macOS-clang-arm64/lib
        /opt/homebrew/Cellar/freetype/2.13.2/lib)
    target_link_libraries(zappy_gui sfml-graphics sfml-window sfml-system freetype)
    set_target_properties(zappy_gui PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "/opt/homebrew/Cellar/freetype/2.13.2/lib")

elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Include directories
    #include_directories("includes/")
    target_include_directories(zappy_gui PRIVATE ${CMAKE_SOURCE_DIR}/GUI/includes)

    # Find and link libraries
    find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
    target_link_libraries(zappy_gui sfml-graphics sfml-window sfml-system)
    target_link_directories(zappy_gui PRIVATE /usr/local/lib)
endif()

# Reconfigurer la cible avec la liste complète des fichiers sources mise à jour
set_target_properties(zappy_gui PROPERTIES SOURCES "${SOURCE_FILES}")
