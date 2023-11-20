cmake_minimum_required(VERSION 3.24)
project(platformer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic")

include_directories(external/include)

link_directories(${CMAKE_BINARY_DIR}/bin)
file(COPY "external/lib/" DESTINATION "bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(platformer main.cpp objects/GameObject.cpp objects/GameObject.h objects/components/Component.cpp objects/components/Component.h objects/components/BoxCollider.h objects/components/BoxCollider.cpp objects/components/RigidBody.cpp objects/components/RigidBody.h objects/components/Player.cpp objects/components/Player.h objects/components/Transform.cpp objects/components/Transform.h objects/components/SpriteRenderer.cpp objects/components/SpriteRenderer.h objects/GameObjectManager.cpp objects/GameObjectManager.h
  math/Vec2.h
  math/Vec3.h)

target_link_libraries(platformer PRIVATE
  sfml-audio
  sfml-graphics
  sfml-main
  sfml-network
  sfml-system
  sfml-window
)