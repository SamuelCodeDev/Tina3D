if(ENABLE_XAUDIO2)
    set(HEADER_FILES XAudio2/include)
    set(SOURCE_FILES XAudio2/Audio.cpp XAudio2/Sound.cpp)
endif()

add_library(audio STATIC ${SOURCE_FILES})
target_include_directories(audio PUBLIC ${HEADER_FILES})
target_include_directories(audio PRIVATE ../include)