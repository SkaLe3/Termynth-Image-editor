cmake_minimum_required(VERSION 3.21)
project(TermynthImageEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Let user pass Qt installation path, otherwise fallback to system default
if(NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "" CACHE PATH "Path to Qt installation")
endif()

# Find required Qt modules
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Source files
file(GLOB_RECURSE CPP_FILES src/*.cpp src/*.h)
file(GLOB_RECURSE UI_FILES src/*.ui)

# Create executable
add_executable(TermynthImageEditor ${CPP_FILES} ${UI_FILES})

# Link Qt libraries
target_link_libraries(TermynthImageEditor PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)


if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS $ENV{Qt6_DIR}/../../bin)
    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET TermynthImageEditor POST_BUILD
            COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:TermynthImageEditor>
            COMMENT "Deploying Qt DLLs"
        )
    else()
        message(WARNING "windeployqt not found; DLLs may be missing")
    endif()
endif()