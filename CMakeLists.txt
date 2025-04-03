cmake_minimum_required(VERSION 3.22)
project(Bombit) #neu doi ten phai vao menu va reload lai

set(CMAKE_CXX_STANDARD 20)

# Must set the path to the main.cpp, for example: scripts/main.cpp if it is inside a folder
set(SOURCE_FILES
        scripts/main.cpp
        scripts/Player/player.cpp
        scripts/Player/player.h
        scripts/Object/bomb.cpp
        scripts/Object/bomb.h
        scripts/Object/explosion.cpp
        scripts/Object/explosion.h
        scripts/Map/map.cpp
        scripts/Map/map.h
        scripts/game.cpp
        scripts/game.h
        scripts/Render/resources.cpp
        scripts/Render/resources.h
        scripts/Info/logic.cpp
        scripts/Info/logic.h
        scripts/Enemy/enemy.cpp
        scripts/Enemy/enemy.h
        scripts/Input/InputHandler.cpp
        scripts/Input/InputHandler.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# --- SDL2 SETUP ---
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(SDL2_PATH "SDL2/x86_64-w64-mingw32")
set(SDL2_IMAGE_PATH "SDL2/x86_64-w64-mingw32")
set(SDL2_TTF_PATH "SDL2/x86_64-w64-mingw32")
set(SDL2_MIXER_PATH "SDL2/x86_64-w64-mingw32")

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${SDL_2_MIXER_INCLUDE_DIR})
find_package(SDL2_image REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES})

target_link_libraries(${CMAKE_PROJECT_NAME} kernel32)


install(TARGETS Bombit DESTINATION bin)
install(DIRECTORY cmake-build-debug/assets DESTINATION .)
