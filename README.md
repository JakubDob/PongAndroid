# Introduction

A clone of the pong game compiled as a native library.

# Prerequisites
Installed [Android NDK](https://developer.android.com/ndk), [Ninja](https://ninja-build.org/) build system, [CMake](https://cmake.org/).
# Building
To build an external native library a toolchain file is required.  
Example configuration:  
Configure [PongGame](https://github.com/JakubDob/Pong) with a powershell script targeting android 6:
```powershell
$system='-DCMAKE_SYSTEM_NAME=Android'
$platform='-DANDROID_PLATFORM=android-23'
$abi='-DCMAKE_ANDROID_ARCH_ABI=armeabi-v7a'
$buildType='-DCMAKE_BUILD_TYPE="Release"'
$genPath='-DCMAKE_MAKE_PROGRAM="Path/to/ninja.exe"'
$ndk='-DANDROID_NDK=Path/to/android-ndk-r23b"'
$tc='-DCMAKE_TOOLCHAIN_FILE="Path/to/android-ndk-r23b/build/cmake/android.toolchain.cmake"'
$find='-DCMAKE_FIND_ROOT_PATH="Path/to/sysroot'
$gen='-G "Ninja"'
$src='-S .'
$dst='-B ./out/android_build'

$args=@($system, $platform, $abi, $buildType, $genPath, $find, $tc, $ndk, $gen, $src, $dst)

cmake $args
```
Build:
```
ninja -C out/android_build
```
This creates libPong.a in the out/android_build directory.  

To build the main shared library, edit `gradle.properties` and set:  
`CMAKE_FIND_ROOT_PATH` and `NDK_PATH`  

Then run

```
./gradlew clean

./gradlew installDebug
or
./gradlew assembleRelease
```
