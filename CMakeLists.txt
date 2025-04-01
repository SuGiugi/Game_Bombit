cmake_minimum_required(VERSION 3.22)
project(Bombit) #neu doi ten phai vao menu va reload lai

set(CMAKE_CXX_STANDARD 20)

# Must set the path to the main.cpp, for example: scripts/main.cpp if it is inside a folder
set(SOURCE_FILES
        scripts/main.cpp
        scripts/player.cpp
        scripts/player.h
        scripts/object/bomb.cpp
        scripts/object/bomb.h
        scripts/object/explosion.cpp
        scripts/object/explosion.h
        scripts/object/item.cpp
        scripts/object/item.h
        scripts/map.cpp
        scripts/map.h
        scripts/game.cpp
        scripts/game.h
        scripts/render/resources.cpp
        scripts/render/resources.h
        scripts/logic.cpp
        scripts/logic.h
        scripts/Enemy/enemy.cpp
        scripts/Enemy/enemy.h
        scripts/input/InputHandler.cpp
        scripts/input/InputHandler.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# --- SDL2 SETUP ---
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(SDL2_PATH "SDL2/x86_64-w64-mingw32")
set(SDL2_IMAGE_PATH "SDL2/x86_64-w64-mingw32")
set(SDL2_TTF_PATH "SDL2/x86_64-w64-mingw32")

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
find_package(SDL2_image REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

target_link_libraries(${CMAKE_PROJECT_NAME} kernel32)


install(TARGETS Bombit DESTINATION bin)
install(DIRECTORY cmake-build-debug/assets DESTINATION .)
