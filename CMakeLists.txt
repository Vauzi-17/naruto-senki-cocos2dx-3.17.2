#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
# ****************************************************************************/

cmake_minimum_required(VERSION 3.6)

set(APP_NAME NarutoSenki317)

project(${APP_NAME})

if(ANDROID)
    add_definitions(-DANDROID)
endif()

set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
)

if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

# ============================================================
# Naruto Senki source files
# ============================================================
list(APPEND GAME_SOURCE
    Classes/AppDelegate.cpp
    Classes/ActionButton.cpp
    Classes/ActionManager.cpp
    Classes/BGLayer.cpp
    Classes/Characters.cpp
    Classes/CreditsLayer.cpp
    Classes/Effect.cpp
    Classes/Element.cpp
    Classes/GameLayer.cpp
    Classes/GameOver.cpp
    Classes/GameScene.cpp
    Classes/GearLayer.cpp
    Classes/HPBar.cpp
    Classes/HudLayer.cpp
    Classes/JoyStick.cpp
    Classes/LoadLayer.cpp
    Classes/NetworkLayer.cpp
    Classes/PauseLayer.cpp
    Classes/RankingLayer.cpp
    Classes/SelectLayer.cpp
    Classes/SkillLayer.cpp
    Classes/StartMenu.cpp

    Classes/MyUtils/CCScrewLayer.cpp
    Classes/MyUtils/CCShake.cpp
    Classes/MyUtils/CCStrokeLabel.cpp
    Classes/MyUtils/KTools.cpp
    Classes/MyUtils/MD5ChecksumDefines.cpp
    Classes/MyUtils/sqlite3.c
)

list(APPEND GAME_HEADER
    Classes/AppDelegate.h
    Classes/ActionButton.h
    Classes/ActionManager.h
    Classes/AppMacros.h
    Classes/BGLayer.h
    Classes/Characters.h
    Classes/CreditsLayer.h
    Classes/Defines.h
    Classes/Effect.h
    Classes/Element.h
    Classes/GameLayer.h
    Classes/GameOver.h
    Classes/GameScene.h
    Classes/GearLayer.h
    Classes/HPBar.h
    Classes/HudLayer.h
    Classes/JoyStick.h
    Classes/LoadLayer.h
    Classes/NetworkLayer.h
    Classes/PauseLayer.h
    Classes/RankingLayer.h
    Classes/SelectLayer.h
    Classes/SkillLayer.h
    Classes/StartMenu.h

    Classes/MyUtils/CCScrewLayer.h
    Classes/MyUtils/CCShake.h
    Classes/MyUtils/CCStrokeLabel.h
    Classes/MyUtils/CMD5Checksum.h
    Classes/MyUtils/KTools.h
    Classes/MyUtils/MD5ChecksumDefines.h
    Classes/MyUtils/sqlite3.h
    Classes/MyUtils/sqlite3ext.h
)

if(ANDROID)
    # Do not change this blindly. It must match the Android template/native library setup.
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE
        proj.android/app/jni/hellocpp/main.cpp
    )
elseif(LINUX)
    list(APPEND GAME_SOURCE
        proj.linux/main.cpp
    )
elseif(WINDOWS)
    list(APPEND GAME_HEADER
        proj.win32/main.h
        proj.win32/resource.h
    )
    list(APPEND GAME_SOURCE
        proj.win32/main.cpp
        proj.win32/game.rc
        ${common_res_files}
    )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
            proj.ios_mac/ios/AppController.h
            proj.ios_mac/ios/RootViewController.h
        )
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets
        )
        list(APPEND GAME_SOURCE
            proj.ios_mac/ios/main.m
            proj.ios_mac/ios/AppController.mm
            proj.ios_mac/ios/RootViewController.mm
            proj.ios_mac/ios/Prefix.pch
            ${APP_UI_RES}
        )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist
        )
        list(APPEND GAME_SOURCE
            proj.ios_mac/mac/main.cpp
            proj.ios_mac/mac/Prefix.pch
            ${APP_UI_RES}
        )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# mark app compile info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
)

if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
endif()

target_link_libraries(${APP_NAME} cocos2d)

target_include_directories(${APP_NAME}
    PRIVATE Classes
    PRIVATE Classes/MyUtils
    PRIVATE ${COCOS2DX_ROOT_PATH}/external
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
    PRIVATE ${COCOS2DX_ROOT_PATH}/extensions
)

target_compile_definitions(${APP_NAME}
    PRIVATE _CRT_SECURE_NO_WARNINGS
)

# mark app resources
setup_cocos_app_config(${APP_NAME})

if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    if(MACOSX)
        set_target_properties(${APP_NAME} PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist"
        )
    elseif(IOS)
        cocos_pak_xcode(${APP_NAME} INFO_PLIST "iOSBundleInfo.plist.in")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
        set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "")
        set_xcode_property(${APP_NAME} CODE_SIGN_IDENTITY "iPhone Developer")
    endif()
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

if(LINUX OR WINDOWS)
    set(APP_RES_DIR "$<TARGET_FILE_DIR:${APP_NAME}>/Resources")
    cocos_copy_target_res(${APP_NAME} COPY_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()
