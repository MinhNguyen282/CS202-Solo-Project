cmake_minimum_required(VERSION 3.16)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(CMakeSFMLProject main.cpp
src/Application.cpp
src/Button.cpp
src/Command.cpp
src/CommandQueue.cpp
src/Component.cpp
src/Container.cpp
src/CreditState.cpp
src/DataTables.cpp
src/Entity.cpp
src/GameState.cpp
src/Label.cpp
src/LoadingState.cpp
src/MechaRockBoss.cpp
src/MenuState.cpp
src/ParallelTask.cpp
src/PauseState.cpp
src/Player.cpp
src/Projectile.cpp
src/SceneNode.cpp
src/SettingState.cpp
src/SpriteNode.cpp
src/State.cpp
src/StateStack.cpp
src/TextNode.cpp
src/TitleState.cpp
src/Utility.cpp
src/Witch.cpp
src/World.cpp
src/GameOverState.cpp
)
target_include_directories(CMakeSFMLProject PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(CMakeSFMLProject PRIVATE sfml-graphics)
target_compile_features(CMakeSFMLProject PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET CMakeSFMLProject
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:CMakeSFMLProject>
        VERBATIM)
endif()

install(TARGETS CMakeSFMLProject)
