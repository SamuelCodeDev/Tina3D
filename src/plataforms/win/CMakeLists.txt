# the audio library
if (ENABLE_XAUDIO2)
    add_subdirectory(audio)
endif ()

# the graphics library
add_subdirectory(graphics)

set(SOURCE_FILES Window.cpp
    Engine.cpp
    Error.cpp
    Game.cpp
    Input.cpp
    Timer.cpp)

# the window library
add_library(window STATIC ${SOURCE_FILES})
target_include_directories(window PUBLIC include)
target_link_libraries(window PRIVATE graphics)