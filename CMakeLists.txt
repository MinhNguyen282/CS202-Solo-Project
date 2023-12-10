cmake_minimum_required(VERSION 3.0.0)
project(LittleWitchAdventure VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)   
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(LittleWitchAdventure main.cpp
src/SceneNode/Command.cpp
src/SceneNode/CommandQueue.cpp
src/SceneNode/Utility.cpp
src/SceneNode/Entity.cpp
src/SceneNode/ParallelTask.cpp
src/SceneNode/SceneNode.cpp
src/SceneNode/SpriteNode.cpp

src/Shader/PostEffect.cpp
src/Shader/BloomEffect.cpp

src/GUI/Component.cpp
src/GUI/Button.cpp
src/GUI/Container.cpp
src/GUI/Label.cpp

src/Data/DataTables.cpp

src/Entity/Enemy.cpp
src/Entity/MechaRockBoss.cpp
src/Entity/Player.cpp
src/Entity/Projectile.cpp
src/Entity/TextNode.cpp
src/Entity/Witch.cpp
src/Entity/ParticleNode.cpp

src/World.cpp

src/State/GameOverState.cpp
src/State/TitleState.cpp
src/State/State.cpp
src/State/StateStack.cpp
src/State/SettingState.cpp
src/State/MenuState.cpp
src/State/GameState.cpp
src/State/Application.cpp
src/State/PauseState.cpp
src/State/CreditState.cpp
src/State/LoadingState.cpp
)
target_include_directories(LittleWitchAdventure PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(LittleWitchAdventure PRIVATE sfml-graphics)
target_compile_features(LittleWitchAdventure PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET LittleWitchAdventure
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:LittleWitchAdventure>
        VERBATIM)
endif()

install(TARGETS LittleWitchAdventure)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
