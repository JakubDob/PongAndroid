cmake_minimum_required(VERSION 3.22.1)

project(PongAndroid)
add_library(PongGame SHARED)
target_compile_features(PongGame PRIVATE cxx_std_20)

add_subdirectory(app/jni)

find_package(Pong REQUIRED)

target_link_libraries(
    PongGame
    PRIVATE
    Pong::Pong
    android
    log
    z
)


