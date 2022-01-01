# WARNING: This file is used for CLion to collect Symbol Information.
# It does not support to compile! Please Initialize the project in Visual Studio and build the project in it too.

cmake_minimum_required(VERSION 3.21)
project(TemplatePlugin)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DUNICODE -DNDEBUG -DWIN32_LEAN_AND_MEAN -D_CRT_SECURE_NO_WARNINGS -D_WINDOWS -D_USRDLL -D_AMD64_ -DNOMINMAX)

file(GLOB_RECURSE SRC_FILES_DIR
        ${PROJECT_SOURCE_DIR}/SDK/Header/*.cpp
        ${PROJECT_SOURCE_DIR}/SDK/Header/*.hpp
        ${PROJECT_SOURCE_DIR}/SDK/Header/*.h
        ${PROJECT_SOURCE_DIR}/Template/*.cpp
        ${PROJECT_SOURCE_DIR}/Template/*.hpp
        ${PROJECT_SOURCE_DIR}/Template/*.h
        )

include_directories(SDK/Header)
include_directories(SDK/Header/third-party)
include_directories(Template)

link_directories(.)

add_library(TemplatePlugin SHARED ${SRC_FILES_DIR})
