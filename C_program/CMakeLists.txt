cmake_minimum_required(VERSION 3.10)
project(Buuble_Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontrar SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Archivos fuente
set(SOURCES
    src/main.cpp
    src/Carta.cpp
    src/Jugador.cpp
    src/Menu.cpp
    src/JuegoVisual.cpp
)

# Crear ejecutable
add_executable(juego ${SOURCES})

# Incluir directorios
target_include_directories(juego PRIVATE include/)

# Linkear SFML
target_link_libraries(juego PRIVATE sfml-graphics sfml-window sfml-system)

# Copiar archivos de fuente (.ttf) a directorio de build
configure_file(arial.ttf arial.ttf COPYONLY)
configure_file(ariblk.ttf ariblk.ttf COPYONLY)
