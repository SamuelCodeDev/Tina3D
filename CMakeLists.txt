cmake_minimum_required(VERSION 3.5)
project(Tina3D LANGUAGES CXX)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

option (ENABLE_EXAMPLES "Enable examples" ON)

if(WIN32)
    option (ENABLE_DIRECTX11 "Enable Directx11 support." ON)
    option (ENABLE_DIRECTX12 "Enable Directx12 support." OFF)
    option (ENABLE_XAUDIO2 "Enable XAudio2 support." ON)
endif()

add_subdirectory(src/plataforms)

if(ENABLE_AUDIO)
    set(TINA_LIBRARIES window graphics audio)
else()
    set(TINA_LIBRARIES window graphics)
endif()

if(ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()