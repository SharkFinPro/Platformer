cmake_minimum_required(VERSION 3.24)
project(platformer)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME}
  main.cpp
  objects/Object.cpp
  objects/Object.h
  objects/components/Component.cpp
  objects/components/Component.h
  objects/components/RigidBody.cpp
  objects/components/RigidBody.h
  objects/components/Player.cpp
  objects/components/Player.h
  objects/components/Transform.cpp
  objects/components/Transform.h
  objects/components/SpriteRenderer.cpp
  objects/components/SpriteRenderer.h
  objects/ObjectManager.cpp
  objects/ObjectManager.h
  math/Vec2.h
  math/Vec3.h
  objects/components/collisions/Collider.cpp
  objects/components/collisions/Collider.h
  objects/components/collisions/MeshCollider.cpp
  objects/components/collisions/MeshCollider.h
  objects/components/collisions/Simplex.cpp
  objects/components/collisions/Simplex.h
  objects/components/Components.h
)

if (MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -pedantic)
endif()

include(FetchContent)

set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)
FetchContent_Declare(
  sfml
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.1
)

FetchContent_MakeAvailable(sfml)

target_link_libraries(${PROJECT_NAME} PRIVATE
  sfml-graphics
)