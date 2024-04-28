cmake_minimum_required(VERSION 3.28)

add_executable(Engine
    "Main.cpp"
)

if(WIN32)
    set_target_properties(Engine PROPERTIES WIN32_EXECUTABLE TRUE)
    set_target_properties(Engine PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()
