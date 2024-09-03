set(SOURCE_FILES GameDemo.cpp
    main.cpp)

if(WIN32)
    set(RESOURCE_PATH ../../resources)
    set(RESOURCES_FILES ${RESOURCE_PATH}/Resources.h 
        ${RESOURCE_PATH}/Resources.rc)
    
    add_executable(simpleWindow WIN32 ${SOURCE_FILES} ${RESOURCES_FILES})
    target_include_directories(simpleWindow PRIVATE ${RESOURCE_PATH})
else()
    add_executable(simpleWindow ${SOURCE_FILES})
endif()

target_include_directories(simpleWindow PRIVATE include)
target_link_libraries(simpleWindow PRIVATE ${TINA_LIBRARIES})