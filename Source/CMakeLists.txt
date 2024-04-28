cmake_minimum_required(VERSION 3.28)

add_executable(Engine
    "Main.cpp"
    "Common/Debug.cpp"
)

target_include_directories(Engine
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../Include"
)

if(WIN32)
    set_target_properties(Engine PROPERTIES WIN32_EXECUTABLE TRUE)
    set_target_properties(Engine PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()
