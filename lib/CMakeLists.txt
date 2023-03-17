set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)
add_subdirectory(SFML)

# imgui contains no CMakeLists, but it's build by imgui-sfml
set(imgui_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)

add_subdirectory(imgui-sfml)

# set(Boost_USE_STATIC_LIBS OFF) 
# set(Boost_USE_MULTITHREADED ON)  
# set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.78)

