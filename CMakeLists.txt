cmake_minimum_required(VERSION 3.22.1)

project(z-flash 
    LANGUAGES CXX
    VERSION 1.0.0
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS FALSE)

# FetchUI
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)


FetchContent_Declare(ftxui
  GIT_REPOSITORY "https://github.com/ArthurSonzogni/ftxui"
  GIT_TAG "a715a767b56245024dfa7469f301e4f39b866ee1"
  GIT_PROGRESS TRUE
  GIT_SHALLOW
  EXCLUDE_FROM_ALL
)

FetchContent_Declare(json 
URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)

FetchContent_MakeAvailable(ftxui)
FetchContent_MakeAvailable(json)
 
# add_executable(z-flash src/uitest.cpp)
add_executable(z-flash 
src/main.cpp
src/api.cpp
)


find_package( CURL REQUIRED )

target_link_libraries(z-flash
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE nlohmann_json::nlohmann_json
  PRIVATE CURL::libcurl
)

set_target_properties(z-flash PROPERTIES CXX_STANDARD 20)
