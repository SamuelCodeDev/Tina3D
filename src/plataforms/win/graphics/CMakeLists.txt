if(ENABLE_DIRECTX11)
    set(HEADER_FILES directx/dx11/include)
    set(SOURCE_FILES directx/dx11/Graphics.cpp)
elseif(ENABLE_DIRECTX12)
    set(HEADER_FILES directx/dx12/include)
    set(SOURCE_FILES directx/dx12/Graphics.cpp)
endif()

add_library(graphics STATIC ${SOURCE_FILES})
target_include_directories(graphics PRIVATE ../include)
target_include_directories(graphics PUBLIC ${HEADER_FILES})